variable "project_id" {
  description = "Google Cloud Project ID"
  type        = string
}

variable "region" {
  description = "Google Cloud region (us-central1 is Free Tier eligible for e2-micro)"
  type        = string
  default     = "us-central1"
}

variable "zone" {
  description = "Google Cloud zone"
  type        = string
  default     = "us-central1-a"
}

variable "vm_arch" {
  description = "Target architecture for the EOS VM ('x86_64' for e2-micro Free Tier, or 'arm64' for t2a-standard-1)"
  type        = string
  default     = "x86_64"
  validation {
    condition     = contains(["x86_64", "arm64"], var.vm_arch)
    error_message = "vm_arch must be either 'x86_64' or 'arm64'."
  }
}

variable "vm_image" {
  description = "Boot disk image family or custom Arch Linux image self_link"
  type        = string
  default     = ""
}

variable "spot_instance" {
  description = "Use Spot VM provisioning model for additional cost savings"
  type        = bool
  default     = false
}

variable "enable_cloud_run" {
  description = "Deploy the optional scale-to-zero Cloud Run showcase service"
  type        = bool
  default     = false
}

variable "cloud_run_image" {
  description = "Container image URI for the optional Cloud Run service"
  type        = string
  default     = "us-docker.pkg.dev/cloudrun/container/hello"
}
