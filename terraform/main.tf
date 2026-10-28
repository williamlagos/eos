locals {
  common_labels = {
    project    = "eos"
    managed_by = "terraform"
    profile    = "cloud"
  }

  machine_type = var.vm_arch == "arm64" ? "t2a-standard-1" : "e2-micro"
  default_image = (
    var.vm_image != "" ? var.vm_image :
    var.vm_arch == "arm64" ? "debian-cloud/debian-12-arm64" : "debian-cloud/debian-12"
  )
}

# ---------------------------------------------------------------------------
# 1. Required Google Cloud APIs
# ---------------------------------------------------------------------------
resource "google_project_service" "apis" {
  for_each = toset([
    "artifactregistry.googleapis.com",
    "compute.googleapis.com",
    "iam.googleapis.com",
    "iap.googleapis.com",
    "run.googleapis.com",
  ])

  project            = var.project_id
  service            = each.value
  disable_on_destroy = false
}

# ---------------------------------------------------------------------------
# 2. Multi-Arch Artifact Registry (eos:base, eos:iot, eos:cloud)
# ---------------------------------------------------------------------------
resource "google_artifact_registry_repository" "eos" {
  location      = var.region
  repository_id = "eos"
  description   = "Multi-arch OCI container repository for EOS (x86_64 and arm64)"
  format        = "DOCKER"
  labels        = local.common_labels

  cleanup_policies {
    id     = "delete-untagged-after-7d"
    action = "DELETE"
    condition {
      tag_state  = "UNTAGGED"
      older_than = "604800s"
    }
  }

  cleanup_policies {
    id     = "keep-latest-3-versions"
    action = "KEEP"
    most_recent_versions {
      keep_count = 3
    }
  }

  depends_on = [google_project_service.apis]
}

# ---------------------------------------------------------------------------
# 3. Least-Privilege IAM Service Account
# ---------------------------------------------------------------------------
resource "google_service_account" "eos_node" {
  account_id   = "eos-node-sa"
  display_name = "EOS Workload Service Account"
  description  = "Least-privilege identity for EOS Compute Engine VM and Cloud Run"
}

resource "google_project_iam_member" "eos_node_roles" {
  for_each = toset([
    "roles/artifactregistry.reader",
    "roles/logging.logWriter",
    "roles/monitoring.metricWriter",
  ])

  project = var.project_id
  role    = each.value
  member  = "serviceAccount:${google_service_account.eos_node.email}"
}

# ---------------------------------------------------------------------------
# 4. VPC Network & Zero-Trust / Web Firewall Rules
# ---------------------------------------------------------------------------
resource "google_compute_network" "eos_vpc" {
  name                    = "eos-vpc"
  auto_create_subnetworks = false
  depends_on              = [google_project_service.apis]
}

resource "google_compute_subnetwork" "eos_subnet" {
  name                     = "eos-subnet-${var.region}"
  ip_cidr_range            = "10.42.0.0/24"
  region                   = var.region
  network                  = google_compute_network.eos_vpc.id
  private_ip_google_access = true
}

# Allow SSH (:22) and Synapse libsockets TCP (:9460) ONLY via IAP TCP Forwarding & internal subnet
resource "google_compute_firewall" "allow_iap_and_internal_control" {
  name    = "eos-allow-iap-control"
  network = google_compute_network.eos_vpc.name

  allow {
    protocol = "tcp"
    ports    = ["22", "9460"]
  }

  source_ranges = [
    "35.235.240.0/20", # Google Cloud IAP TCP forwarding CIDR
    "10.42.0.0/24",    # Internal EOS VPC subnet
  ]

  target_tags = ["eos-node"]
}

# Allow public HTTP/HTTPS (:80, :443) ONLY to web-tagged nodes
resource "google_compute_firewall" "allow_public_web" {
  name    = "eos-allow-public-web"
  network = google_compute_network.eos_vpc.name

  allow {
    protocol = "tcp"
    ports    = ["80", "443"]
  }

  source_ranges = ["0.0.0.0/0"]
  target_tags   = ["eos-web"]
}

# ---------------------------------------------------------------------------
# 5. Stateful EOS Compute Engine VM (x86_64 or arm64)
# ---------------------------------------------------------------------------
resource "google_compute_instance" "eos_vm" {
  name         = "eos-cloud-vm"
  machine_type = local.machine_type
  zone         = var.zone
  tags         = ["eos-node", "eos-web"]
  labels       = local.common_labels

  boot_disk {
    initialize_params {
      image = local.default_image
      size  = 20
      type  = "pd-standard"
    }
  }

  network_interface {
    subnetwork = google_compute_subnetwork.eos_subnet.id
    access_config {
      network_tier = "STANDARD"
    }
  }

  metadata = {
    enable-oslogin = "TRUE"
  }

  shielded_instance_config {
    enable_secure_boot          = true
    enable_vtpm                 = true
    enable_integrity_monitoring = true
  }

  dynamic "scheduling" {
    for_each = var.spot_instance ? [1] : []
    content {
      preemptible                 = true
      provisioning_model          = "SPOT"
      automatic_restart           = false
      instance_termination_action = "STOP"
    }
  }

  service_account {
    email  = google_service_account.eos_node.email
    scopes = ["cloud-platform"]
  }
}

# ---------------------------------------------------------------------------
# 6. Optional Scale-to-Zero Cloud Run Service (Companion / Stateless Apps)
# ---------------------------------------------------------------------------
resource "google_cloud_run_v2_service" "eos_serverless" {
  count               = var.enable_cloud_run ? 1 : 0
  name                = "eos-cloud-service"
  location            = var.region
  deletion_protection = false
  ingress             = "INGRESS_TRAFFIC_ALL"
  labels              = local.common_labels

  template {
    service_account = google_service_account.eos_node.email

    scaling {
      min_instance_count = 0
      max_instance_count = 2
    }

    containers {
      image = var.cloud_run_image
      resources {
        limits = {
          cpu    = "1"
          memory = "512Mi"
        }
      }
    }
  }

  depends_on = [google_project_service.apis]
}
