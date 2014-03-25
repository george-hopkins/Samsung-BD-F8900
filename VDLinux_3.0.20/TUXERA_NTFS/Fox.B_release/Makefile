# Rules for making Tuxera NTFS.
#
# Copyright (c) 2009-2011 Anton Altaparmakov & Tuxera Inc. All rights reserved.
#
# You can set various options for the NTFS driver here:

# Set the version number string.
CONFIG_NTFS_VERSION = 3011.11.4

# Set this to the name of the generated file system you want.  This will also
# become the kernel module name.  Normally this is just "tntfs".
CONFIG_NTFS_NAME = tntfs

# Uncomment the below to use full Unicode support thus interpreting the NTFS
# characters to be stored in utf16 rather than UCS2 and assuming that the Linux
# side is using utf8.
CONFIG_NTFS_UTF8 = y

# Set the below to the default NLS option to use.  If commented out the driver
# will use the kernel wide default provided by CONFIG_NLS_DEFAULT.  Usually the
# desired value is utf8.  Note the kernel will need to have this NLS character
# set enabled otherwise mounting will fail.  You can override this default at
# mount time by supplying the nls=charset mount option.
CONFIG_NTFS_NLS_DEFAULT = utf8

# Set the below to the default ioblocksize option to use (in bytes).  If
# commented out the driver will use the kernel wide default which is the
# PAGE_SIZE of the machine which on most normal PCs is 4096 bytes.  You can
# override this default at mount time by supplying the ioblocksize=size mount
# option where size is the desired ioblocksize.
CONFIG_NTFS_IOBLOCKSIZE_DEFAULT = 131072

# Uncomment the below to get debugging code compiled in.  Note this will slow
# down the driver so do not enable it for production / performance testing.
#CONFIG_NTFS_DEBUG = y

# Uncomment the below to enable the readpages related code.
CONFIG_NTFS_READPAGES = y

# Uncomment the below to enable the new writepages related code.
CONFIG_NTFS_WRITEPAGES = y

# Uncomment this to enable transaction based index operations 
CONFIG_NTFS_INDEX_TRANSACTIONS = y

# Uncomment this to enable the NTFS syncd kernel thread daemon which causes
# specific inodes to be written out to disk immediately but asynchronously,
# i.e. this is as close to "-o sync" as possible without the performance impact
# of "-o sync".
CONFIG_NTFS_SYNCD = n

# By default, allow up to a maximum of 100ms (i.e. 0.1 second) in between an
# inode being scheduled for syncing and ntfssyncd starting to process it.
#
# By never allowing ntfssyncd to sleep for more than CONFIG_NTFS_SYNCD_MSECS
# milliseconds, we ensure that ntfssyncd is woken up after 
# CONFIG_NTFS_SYNCD_MSECS milliseconds and the inode is written out then.  
# If we did not have a timeout on the sleep then the inode would not be 
# written out until another inode is scheduled for syncing.
#
# This default setting can be overriden with the syncd_max_sleep_msecs
# mount option.
CONFIG_NTFS_SYNCD_MSECS = 100

# Set the below to the default, minimum size to use for read and write
# iostreaming (in bytes). If commented out the driver will always try to use
# iostreaming when it is enabled. Note this does not affect real O_DIRECT i/o
# requests as they are always, unconditonally done using iostreaming. You can
# override this default at mount time by supplying the
# min_iostreaming_read_iosize=size (aka
# min_nocache_read_iosize=size aka min_directio_read_iosize=size) and
# min_iostreaming_write_iosize=size (aka
# min_nocache_write_iosize=size aka min_directio_write_iosize=size) mount
# options where size is the desired minimum size to use for iostreaming.
CONFIG_NTFS_MIN_NOCACHE_READ_IOSIZE_DEFAULT = 131072
CONFIG_NTFS_MIN_NOCACHE_WRITE_IOSIZE_DEFAULT = 65536

# Uncomment this to reset the journal and thus allowing read-write mounting 
# even when the journal is dirty. 
CONFIG_NTFS_RESET_JOURNAL = y

# Uncomment the following to create a commercial build. By default, the module
# is an evaluation version that works for 100,000 seconds after a mount.
#CONFIG_NTFS_COMMERCIAL = y

CONFIG_TNTFS_FS = m
CONFIG_NTFS_RW = y

obj-$(CONFIG_TNTFS_FS) += $(CONFIG_NTFS_NAME).o

$(CONFIG_NTFS_NAME)-objs :=	aops.o \
				attr_list.o \
				attrib.o \
				bitmap.o \
				collate.o \
				compat.o \
				compress.o \
				debug.o \
				dir.o \
				file.o \
				index.o \
				inode.o \
				lcnalloc.o \
				logfile.o \
				mft.o \
				mst.o \
				namei.o \
				quota.o \
				runlist.o \
				secure.o \
				super.o \
				sysctl.o \
				unistr.o \
				usnjrnl.o

EXTRA_CFLAGS = -DNTFS_NAME=\"$(CONFIG_NTFS_NAME)\"

ifneq ($(NTFS_VERSION),)
ifneq ($(NTFS_VERSION),"")
EXTRA_CFLAGS += -DNTFS_VERSION=\"$(NTFS_VERSION)\"
else
EXTRA_CFLAGS += -DNTFS_VERSION=\"$(CONFIG_NTFS_VERSION)\"
endif
else
EXTRA_CFLAGS += -DNTFS_VERSION=\"$(CONFIG_NTFS_VERSION)\"
endif

ifeq ($(CONFIG_NTFS_UTF8),y)
EXTRA_CFLAGS += -DNTFS_UTF8
endif

ifneq ($(CONFIG_NTFS_NLS_DEFAULT),)
ifneq ($(CONFIG_NTFS_NLS_DEFAULT),"")
EXTRA_CFLAGS += -DNTFS_NLS_DEFAULT=\"$(CONFIG_NTFS_NLS_DEFAULT)\"
endif
endif

ifneq ($(CONFIG_NTFS_IOBLOCKSIZE_DEFAULT),)
ifneq ($(CONFIG_NTFS_IOBLOCKSIZE_DEFAULT),"")
EXTRA_CFLAGS += -DNTFS_IOBLOCKSIZE_DEFAULT=$(CONFIG_NTFS_IOBLOCKSIZE_DEFAULT)
endif
endif

ifeq ($(CONFIG_NTFS_READPAGES),y)
EXTRA_CFLAGS += -DNTFS_READPAGES
endif

ifeq ($(CONFIG_NTFS_WRITEPAGES),y)
EXTRA_CFLAGS += -DNTFS_WRITEPAGES
endif

ifeq ($(CONFIG_NTFS_INDEX_TRANSACTIONS),y)
EXTRA_CFLAGS += -DNTFS_INDEX_TRANSACTIONS
endif

ifeq ($(CONFIG_NTFS_SYNCD),y)
$(CONFIG_NTFS_NAME)-objs += syncd.o
EXTRA_CFLAGS += -DNTFS_SYNCD -DNTFS_SYNCD_MSECS=$(CONFIG_NTFS_SYNCD_MSECS)
endif

ifneq ($(CONFIG_NTFS_MIN_NOCACHE_READ_IOSIZE_DEFAULT),)
ifneq ($(CONFIG_NTFS_MIN_NOCACHE_READ_IOSIZE_DEFAULT),"")
EXTRA_CFLAGS += -DNTFS_MIN_NOCACHE_READ_IOSIZE_DEFAULT=$(CONFIG_NTFS_MIN_NOCACHE_READ_IOSIZE_DEFAULT)
endif
endif

ifneq ($(CONFIG_NTFS_MIN_NOCACHE_WRITE_IOSIZE_DEFAULT),)
ifneq ($(CONFIG_NTFS_MIN_NOCACHE_WRITE_IOSIZE_DEFAULT),"")
EXTRA_CFLAGS += -DNTFS_MIN_NOCACHE_WRITE_IOSIZE_DEFAULT=$(CONFIG_NTFS_MIN_NOCACHE_WRITE_IOSIZE_DEFAULT)
endif
endif

ifeq ($(CONFIG_NTFS_RESET_JOURNAL),y)
EXTRA_CFLAGS += -DNTFS_RESET_JOURNAL
endif

ifeq ($(NTFS_COMMERCIAL),y)
EXTRA_CFLAGS += -DNTFS_COMMERCIAL
else
ifeq ($(CONFIG_NTFS_COMMERCIAL),y)
EXTRA_CFLAGS += -DNTFS_COMMERCIAL
endif
endif

