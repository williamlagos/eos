output "artifact_registry_url" {
  description = "Multi-arch Docker repository URL in Artifact Registry"
  value       = "${var.region}-docker.pkg.dev/${var.project_id}/${google_artifact_registry_repository.eos.repository_id}"
}

output "vm_name" {
  description = "EOS Compute Engine instance name"
  value       = google_compute_instance.eos_vm.name
}

output "vm_public_ip" {
  description = "External IP for HTTP/HTTPS web traffic (:80/:443)"
  value       = google_compute_instance.eos_vm.network_interface[0].access_config[0].nat_ip
}

output "iap_synapse_tunnel_command" {
  description = "Command to tunnel local port 9460 to Synapse TCP control plane via IAP"
  value       = "gcloud compute start-iap-tunnel ${google_compute_instance.eos_vm.name} 9460 --local-host-port=127.0.0.1:9460 --zone=${var.zone} --project=${var.project_id}"
}

output "iap_ssh_tunnel_command" {
  description = "Command to tunnel local port 2224 to VM SSH (:22) for Ansible playbooks/site.yml"
  value       = "gcloud compute start-iap-tunnel ${google_compute_instance.eos_vm.name} 22 --local-host-port=127.0.0.1:2224 --zone=${var.zone} --project=${var.project_id}"
}

output "cloud_run_url" {
  description = "URL of the optional scale-to-zero Cloud Run service"
  value       = var.enable_cloud_run ? google_cloud_run_v2_service.eos_serverless[0].uri : null
}
