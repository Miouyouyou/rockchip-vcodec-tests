#include <stdint.h>
#include <sys/ioctl.h> // ioctl
#include <unistd.h>      // _exit, close
#include <sys/types.h>   // open
#include <sys/stat.h>    // open
#include <fcntl.h>       // open
#include <stdio.h>       // printf / perror

#include <errno.h> // errno, perror

#include "vcodec_service.h"

#define LOG(...) fprintf(stderr, __VA_ARGS__);

static void test_desc(char const * const description) {
	LOG("%s\n", description);
}

static void test_part(const char * const description) {
	LOG("  %s\n", description);
}

void test_get_and_set_registers(int fd) {
	test_desc("Testing IOCTL : VPU_IOC_GET_REG");
	unsigned int request = VPU_IOC_GET_REG;
	unsigned int session_request = VPU_IOC_SET_CLIENT_TYPE;
	struct vpu_request vpu_request = {
		.req = NULL,
		.size = 0
	};
	for (enum VPU_CLIENT_TYPE session_type = VPU_ENC;
	     session_type < VPU_TYPE_BUTT; session_type++) {
		LOG("Setting up session type : %d\n", session_type);
		ioctl(fd, session_request, session_type);
		test_part("Getting the register");
		ioctl(fd, request, &vpu_request);
		LOG("VPU register address (?) : %p\n", vpu_request.req);
		LOG("VPU register size    (?) : %d\n", vpu_request.size);
	}

}

void test_iommu_status(int fd) {
	test_desc("Testing IOCTL : VPU_IOC_PROBE_IOMMU_STATUS");
	int iommu_status;
	ioctl(fd, VPU_IOC_PROBE_IOMMU_STATUS, &iommu_status);
	printf("IOMMU_STATUS ! %d\n", iommu_status);
}

/* This only test if setting the session type doesn't cause a
   big crash. This does not test if the session type is set
   correctly, however. Might have to add an IOCTL for that */
void test_session_type(int fd) {
	test_desc("Testing IOCTL : VPU_IOC_SET_CLIENT_TYPE\n");
	unsigned int request = VPU_IOC_SET_CLIENT_TYPE;
	/* Why are enum in uppercase ? */
	test_part("Session type : VPU_ENC");
	enum VPU_CLIENT_TYPE session_type = VPU_ENC;
	ioctl(fd, request, session_type);
	test_part("Session type : VPU_DEC");
	session_type = VPU_DEC;
	ioctl(fd, request, session_type);
	test_part("Session type : VPU_PP");
	session_type = VPU_PP;
	ioctl(fd, request, session_type);
	test_part("Session type : VPU_DEC_PP");
	session_type = VPU_DEC_PP;
	ioctl(fd, request, session_type);
}

int main() {

  int fd = open("/dev/vpu-service", O_RDWR);
	if (fd >= 0) {
		test_iommu_status(fd);
		test_session_type(fd);
		test_get_and_set_registers(fd);
		close(fd);
	}
	else { perror("Could not open /dev/vpu-service"); }
  return 0;
}

