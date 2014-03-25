#ifndef __CMA_DEV_H
#define __CMA_DEV_H

#define CMA_DEV_NAME "cma"

struct cma_private_data {
	struct cm	*cm;
	unsigned long	size;
	unsigned long	phys;
	struct cmaregion *region;
};

ssize_t cma_region_show_chunks(struct device *dev,
				struct device_attribute *attr, char *buf);
ssize_t cma_region_show_free(struct device *dev,
				struct device_attribute *attr, char *buf);
ssize_t cma_region_show_size(struct device *dev,
				struct device_attribute *attr, char *buf);
ssize_t cma_region_show_start(struct device *dev,
				struct device_attribute *attr, char *buf);
ssize_t cma_region_show_largest(struct device *dev,
				struct device_attribute *attr, char *buf);

#endif
