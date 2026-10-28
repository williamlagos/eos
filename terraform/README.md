# EOS Google Cloud Terraform Showcase

Declarative Terraform configuration for provisioning the **EOS Cloud & IoT Fleet Hub** architecture on Google Cloud.

See [`../solution-architecture-guide.md`](../solution-architecture-guide.md) for the complete architecture blueprint, design rationale, and Well-Architected Framework alignment.

## Provisioned Resources

- **Artifact Registry (`google_artifact_registry_repository.eos`):** Multi-arch (`linux/amd64`, `linux/arm64`) OCI container registry with automated 7-day untagged cleanup and 3-version retention policies.
- **Least-Privilege IAM (`google_service_account.eos_node`):** Dedicated workload identity scoped strictly to `roles/artifactregistry.reader`, `roles/logging.logWriter`, and `roles/monitoring.metricWriter`.
- **VPC & Zero-Trust IAP Firewall (`eos-vpc`, `eos-allow-iap-control`, `eos-allow-public-web`):**
  - Restricts SSH (`:22`) and Synapse's `libsockets` TCP control port (`:9460`) strictly to Google Cloud IAP TCP Forwarding (`35.235.240.0/20`) and internal VPC peers (`10.42.0.0/24`).
  - Exposes only HTTP/HTTPS (`:80`, `:443`) to the public internet on `eos-web`-tagged nodes.
- **Stateful EOS Compute Engine VM (`google_compute_instance.eos_vm`):** Configurable between `e2-micro` (`x86_64` Free Tier) and `t2a-standard-1` (`arm64`), hardened with Shielded VM and OS Login.
- **Optional Cloud Run Service (`google_cloud_run_v2_service.eos_serverless`):** Scale-to-zero (`min_instance_count = 0`) serverless container showcase (`enable_cloud_run = true`).

## Usage

```sh
cd terraform
cp terraform.tfvars.example terraform.tfvars

# Initialize and validate offline (no cloud resources created)
terraform init -backend=false
terraform validate

# Preview or apply against a Google Cloud project
terraform plan
terraform apply
```
