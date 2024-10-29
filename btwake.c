#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/gpio.h>

int main(int argc, char **argv)
{
	struct gpiohandle_request req;
	struct gpiohandle_data data;
	char chrdev_name[20];
	int fd, ret;

	strcpy(chrdev_name, "/dev/gpiochip8");

	/*  Open device: gpiochip8 for GPIO bank I */
	fd = open(chrdev_name, 0);
	if (fd == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to open %s\n", chrdev_name);

		return ret;
	}

	/* HOST wake BT GPIO line: GPIO_I_4 */
	req.lineoffsets[0] = 4;
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	memcpy(req.default_values, &data, sizeof(req.default_values));
	strcpy(req.consumer_label, "host_wake_bt_gpio_I_4");
	req.lines  = 1;

	ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
	if (ret == -1) {
		ret = -errno;
		fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
			ret);
	}
	if (close(fd) == -1)
		perror("Failed to close GPIO character device file");

	/*  Host start wake bt */
        data.values[0] = 1;
        ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
        if (ret == -1) {
        	ret = -errno;
        	fprintf(stderr, "Failed to HIGH host wake bt IO\n");
        }

        sleep(1);

        data.values[0] = 0;
        ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
        if (ret == -1) {
        	ret = -errno;
        	fprintf(stderr, "Failed to LOW host wake bt IO\n");
        }

	sleep(1);

        data.values[0] = 1;
        ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
        if (ret == -1) {
        	ret = -errno;
        	fprintf(stderr, "Failed to HIGH host wake bt IO\n");
        }

	/*  release line */
	ret = close(req.fd);
	if (ret == -1) {
		perror("Failed to close GPIO LINEHANDLE device file");
		ret = -errno;
	}

	return ret;
}
