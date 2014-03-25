
static struct fsg_common *fsg_common_from_dev(struct device *dev)
{
        return container_of(dev, struct fsg_common, dev);
}


static ssize_t fsg_common_show_nluns(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	struct fsg_common *common = fsg_common_from_dev(dev);

	return sprintf(buf, "%u\n", common->nluns);
}


static ssize_t fsg_common_store_nluns(struct device *dev, struct device_attribute *attr,
			      const char *buf, size_t count)
{
	int	rc = -EINVAL;

	return (rc < 0 ? rc : count);	
}

#if 0
int filter_media_command(char* buf, size_t count, char**filename)
{
        int     cmd_state;
        char    *cmdname;

        cmdname = *filename = NULL;
        cmd_state = NOOP_STORAGE_MEDIA;

        /* Remove a trailing newline */
        if (count > 4 && (buf[count-1] == '\n' || buf[count-1] == ' '))
                ((char *) buf)[count-1] = 0;            /* Ugh! */

        /* filter command */
        if((count > 4) && (buf[3] == ' ')) {
                *filename = (char*)buf + 4;
                cmdname = (char*)buf;
                ((char*)buf)[3] = 0;

                if (strcmp(cmdname, "add") == 0) {
                        cmd_state = ADD_STORAGE_MEDIA;

                }
                else if (strcmp(cmdname, "del") == 0) {
                        cmd_state = DELETE_STORAGE_MEDIA;
                }
		else
                	*filename = NULL;
        } 

	return cmd_state;
}
#endif

static ssize_t fsg_common_show_files(struct device *dev, struct device_attribute *attr,
			     char *buf)
{
	struct fsg_common *common = fsg_common_from_dev(dev);
       	struct rw_semaphore     *filesem = dev_get_drvdata(dev);
	struct fsg_lun *curlun = NULL;
        char           	*p;
	int		i, len, rc = 0;

      	len =  PAGE_SIZE - 1;
	down_read(filesem);
 	for (i = 0, curlun = common->luns;  (i < common->nluns && len != 0); ++i, ++curlun) {
		if(fsg_lun_is_open(curlun)) {	/* get the complete name */
 			p = d_path(&curlun->filp->f_path, buf, len);
			if (IS_ERR(p))
				rc = PTR_ERR(p);
			else {
				rc = strlen(p);
				memmove(buf, p, rc);
				buf[rc] = '\n';         /* Add a newline */
				len -= (rc + 1);
				buf += (rc + 1);
			}
		}
	}
	*buf = 0;
  	up_read(filesem);
	return (PAGE_SIZE - len);
}


static ssize_t fsg_common_store_files(struct device *dev, struct device_attribute *attr,
			      const char *buf, size_t count)
{
	struct fsg_common *common = fsg_common_from_dev(dev);
        struct rw_semaphore     *filesem = dev_get_drvdata(dev);
	struct fsg_lun *curlun = NULL;
        int    i, cmd_state, rc = 0; 
        char   *filename;
	cmd_state = filter_media_command((char*)buf, count, &filename);
      	
	down_write(filesem);
	switch(cmd_state) {
	/* Eject current medium */
        	case DELETE_STORAGE_MEDIA:
                /*1. find the first matching slot */
                for(i = 0,curlun = common->luns; i < common->nluns; i++, ++curlun) {
                        if(fsg_lun_is_open(curlun)) {
                        /* first free slot -load media */
				if(strcmp(filename,curlun->filename) == 0) {
                        	/* matching slot -unload media*/
					fsg_lun_close(curlun);
					curlun->unit_attention_data = SS_MEDIUM_NOT_PRESENT;
                        		break;
				}
                        }
                }
                if(i == common->nluns) {
                /* No free slot in port */
                        rc = -EINVAL;
                }
		break;

      	/* Load new medium */
                case ADD_STORAGE_MEDIA:
		/* 1. check if same media already loaded*/
                for(i = 0,curlun = common->luns; i < common->nluns; i++, ++curlun) {
                        if(fsg_lun_is_open(curlun)) {
                        /* first free slot -load media */
				if(strcmp(filename,curlun->filename) == 0) {
                        	/* matching slot -unload media*/
					rc= -EINPROGRESS;
					break;
				}
                        }
                }
		if(rc < 0)
			break;

		/* 2. find the first available slot */		
		for(i = 0,curlun = common->luns; i < common->nluns; i++, ++curlun) {
			if(!fsg_lun_is_open(curlun)) {
			/* first free slot -load media */
				rc = fsg_lun_open(curlun, filename);
				break;
			}			
		}
		if(i == common->nluns) {
		/* No free slot in port */	
			rc = -ENOSPC;	
		}

	       	if (rc == 0)
			curlun->unit_attention_data =
				SS_NOT_READY_TO_READY_TRANSITION;
		break;
		
		default:
		rc = -EINVAL;
		break;
	}
	up_write(filesem);
	
	return (rc < 0 ? rc : count);	
}

