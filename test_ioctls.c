#include <stdint.h>
#include <sys/ioctl.h> // ioctl
#include <unistd.h>      // _exit, close
#include <sys/types.h>   // open
#include <sys/stat.h>    // open
#include <fcntl.h>       // open
#include <stdio.h>       // printf / perror

#include <errno.h> // errno, perror

#include "vcodec_service.h"

int main() {

  int iommu_status;
  int fd = open("/dev/vpu-service", O_RDWR);
	if (fd >= 0) {
		ioctl(fd, VPU_IOC_PROBE_IOMMU_STATUS, &iommu_status);
		printf("IOMMU_STATUS ! %d\n", iommu_status);
		close(fd);
	}
	else { perror("Could not open /dev/vpu-service"); }
  return 0;
}
