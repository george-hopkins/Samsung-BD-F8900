#ifndef PTP_THREAD_SAFE
#define PTPFreeMem free
#endif


/* currently this file is included into ptp.c */

#if defined(ENDIAN_PATCH_BY_SISC)
/* 20090724  SISC by Ajeet*/
#else
#define	ENABLE_ENDIAN_BUG_FIX	/*kyungsik, little endian bug-fix*/
#define RARR1(val,h,t)  //#sariya
#endif

static inline uint16_t
htod16p (PTPParams *params, uint16_t var)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	return ((params->byteorder==PTP_DL_LE) ? var : swap16(var));
#else
	return ((params->byteorder==PTP_DL_LE)?htole16(var):htobe16(var));
#endif
}

static inline uint32_t
htod32p (PTPParams *params, uint32_t var)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	return ((params->byteorder==PTP_DL_LE) ? var : swap32(var));
#else
	return ((params->byteorder==PTP_DL_LE)?htole32(var):htobe32(var));
#endif
}

static inline void
htod16ap (PTPParams *params, unsigned char *a, uint16_t val)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	val = ((params->byteorder==PTP_DL_LE) ? val : swap16(val));
	memcpy(a, &val, 2);
#else
#ifdef ENABLE_ENDIAN_BUG_FIX
	params = NULL;    //intialising unused argument
	memcpy(&val,a,2);
#else
	if (params->byteorder==PTP_DL_LE)
		htole16a(a,val);
	else 
		htobe16a(a,val);
#endif
#endif
}

static inline void
htod32ap (PTPParams *params, unsigned char *a, uint32_t val)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	val = ((params->byteorder==PTP_DL_LE) ? val : swap32(val));
	memcpy(a, &val, 4);
#else
#ifdef ENABLE_ENDIAN_BUG_FIX
	params = NULL;    //intialising unused argument
	val = (*a &0x000000FF)|(*(a+sizeof(unsigned char))<<8)
		|(*(a+2*sizeof(unsigned char))<<16)|(*(a+3*sizeof(unsigned char))<<24);
#else
	if (params->byteorder==PTP_DL_LE)
		htole32a(a,val);
	else 
		htobe32a(a,val);
#endif
#endif
}

static inline uint16_t
dtoh16p (PTPParams *params, uint16_t var)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	return ((params->byteorder==PTP_DL_LE) ? var : swap16(var));
#else
	return ((params->byteorder==PTP_DL_LE)?le16toh(var):be16toh(var));
#endif
}

static inline uint32_t
dtoh32p (PTPParams *params, uint32_t var)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	return ((params->byteorder==PTP_DL_LE) ? var : swap32(var));
#else
	return ((params->byteorder==PTP_DL_LE)?le32toh(var):be32toh(var));
#endif
}

static inline uint16_t
dtoh16ap (PTPParams *params, unsigned char *a)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	uint16_t var=0;
	memcpy(&var, a, 2);
	return ((params->byteorder==PTP_DL_LE) ? var : swap16(var));
#else
#ifdef ENABLE_ENDIAN_BUG_FIX
	params = NULL;    //intialising unused argument
	uint16_t retval;
	memcpy(&retval,a,2);
	return retval;
#else
	return ((params->byteorder==PTP_DL_LE)?le16atoh(a):be16atoh(a));
#endif
#endif
}

static inline uint32_t
dtoh32ap (PTPParams *params, unsigned char *a)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	uint32_t var=0;
	memcpy(&var, a, 4);
	return ((params->byteorder==PTP_DL_LE) ? var : swap32(var));

#else
#ifdef ENABLE_ENDIAN_BUG_FIX	
	params = NULL;    //intialising unused argument
	uint32_t retval;
	retval = (*a &0x000000FF)|(*(a+sizeof(unsigned char))<<8)
		|(*(a+2*sizeof(unsigned char))<<16)|(*(a+3*sizeof(unsigned char))<<24);
	return retval;
#else
	return ((params->byteorder==PTP_DL_LE)?le32atoh(a):be32atoh(a));
#endif
#endif
}

static inline uint64_t
dtoh64ap (PTPParams *params, unsigned char *a)
{
#if defined(ENDIAN_PATCH_BY_SISC)
	uint64_t var=0;
	memcpy(&var, a, 8);
	return ((params->byteorder==PTP_DL_LE) ? var : swap64(var));
#else
	uint64_t retval, temp2;
	uint32_t temp1;
	temp1 = dtoh32ap(params, a);
	temp2 =(uint64_t)dtoh32ap(params,(a+4));
	retval = (temp1 & 0x00000000FFFFFFFF) | (temp2<<32);
	return retval;
#endif
}

#define htod8a(a,x)	*(uint8_t*)(a) = x
#define htod16a(a,x)	htod16ap(params,(unsigned char *)a,x)
#define htod32a(a,x)	htod32ap(params,(unsigned char *)a,x)
#define htod16(x)	htod16p(params,x)
#define htod32(x)	htod32p(params,x)

#define dtoh8a(x)	(*(uint8_t*)(x))

#define dtoh16a(a)	dtoh16ap(params,(unsigned char *)a)
#define dtoh32a(a)	dtoh32ap(params,(unsigned char *)a)
#define dtoh64a(a)	dtoh64ap(params,(unsigned char *)a)	// modified by kskim(2007-11-23), get storage size
#define dtoh16(x)	dtoh16p(params,x)
#define dtoh32(x)	dtoh32p(params,x)


static inline char*
ptp_unpack_string(PTPParams *params, char* data, uint16_t offset, uint8_t *len)
{
	int i;
	char *string=NULL;
	

	*len=dtoh8a(&data[offset]);
	if (*len) {
		string=(char*)malloc(*len); //vishal g++ typecasting
		if(NULL == string)
		{
			return NULL;
		}
		memset(string, 0, *len);
		for (i=0;i<*len && i< PTP_MAXSTRLEN; i++) {
			string[i]=(char)dtoh16a(&data[offset+i*2+1]);
		}
		/* be paranoid! :( */
		string[*len-1]=0;
	}
	return (string);
}

static inline void
ptp_pack_string(PTPParams *params, char *string, char* data, uint16_t offset, uint8_t *len)
{
	int i;
	*len = (uint8_t)strlen(string);
	
	/* XXX: check strlen! */
	htod8a(&data[offset],*len+1);
	for (i=0;i<*len && i< PTP_MAXSTRLEN; i++) {
		htod16a(&data[offset+i*2+1],(uint16_t)string[i]);
	}
}

static inline uint32_t
ptp_unpack_uint32_t_array(PTPParams *params, char* data, uint16_t offset, uint32_t **array)
{
	uint32_t n, i=0;
	 
	n=dtoh32a(&data[offset]);
	
	*array = (uint32_t*)malloc (n*sizeof(uint32_t));//vishal g++ typecasting
	
	 if(NULL != *array)
	 {
		while (n>i) {
			(*array)[i]=dtoh32a(&data[offset+(sizeof(uint32_t)*(i+1))]);	
			i++;
		}
	 }

	return n;
}

static inline uint32_t
ptp_unpack_uint16_t_array(PTPParams *params, char* data, uint16_t offset, uint16_t **array)
{
	uint32_t n, i=0;

	n=dtoh32a(&data[offset]);
	*array = (uint16_t*)malloc (n*sizeof(uint16_t));//vishal g++ typecasting
	if(NULL != *array)
	{
		while (n>i) {
			(*array)[i]=dtoh16a(&data[offset+(sizeof(uint16_t)*(i+2))]);
			i++;
		}
	}
	return n;
}

/* DeviceInfo pack/unpack */

#define PTP_di_StandardVersion		 0
#define PTP_di_VendorExtensionID	 2
#define PTP_di_VendorExtensionVersion	 6
#define PTP_di_VendorExtensionDesc	 8
#define PTP_di_FunctionalMode		 8
#define PTP_di_OperationsSupported	10

static inline void
ptp_unpack_DI (PTPParams *params, char* data, PTPDeviceInfo *di, unsigned int datalen)
{
	uint8_t len;
	unsigned int totallen;
	datalen=0; //allocating unused variable to zero
	
	di->StaqndardVersion = dtoh16a(&data[PTP_di_StandardVersion]);
	di->VendorExtensionID =
		dtoh32a(&data[PTP_di_VendorExtensionID]);
	di->VendorExtensionVersion =
		dtoh16a(&data[PTP_di_VendorExtensionVersion]);
	di->VendorExtensionDesc = 
		ptp_unpack_string(params, data,
		PTP_di_VendorExtensionDesc, &len); 
	totallen=len*2+1;
	di->FunctionalMode = 
		dtoh16a(&data[PTP_di_FunctionalMode+totallen]);
	di->OperationsSupported_len = ptp_unpack_uint16_t_array(params, data,
		PTP_di_OperationsSupported+totallen,
		&di->OperationsSupported);
	totallen=totallen+di->OperationsSupported_len*sizeof(uint16_t)+sizeof(uint32_t);
	di->EventsSupported_len = ptp_unpack_uint16_t_array(params, data,
		PTP_di_OperationsSupported+totallen,
		&di->EventsSupported);
	totallen=totallen+di->EventsSupported_len*sizeof(uint16_t)+sizeof(uint32_t);
	di->DevicePropertiesSupported_len =
		ptp_unpack_uint16_t_array(params, data,
		PTP_di_OperationsSupported+totallen,
		&di->DevicePropertiesSupported);
	totallen=totallen+di->DevicePropertiesSupported_len*sizeof(uint16_t)+sizeof(uint32_t);
	di->CaptureFormats_len = ptp_unpack_uint16_t_array(params, data,
		PTP_di_OperationsSupported+totallen,
		&di->CaptureFormats);
	totallen=totallen+di->CaptureFormats_len*sizeof(uint16_t)+sizeof(uint32_t);
	di->ImageFormats_len = ptp_unpack_uint16_t_array(params, data,
		PTP_di_OperationsSupported+totallen,
		&di->ImageFormats);
	totallen=totallen+di->ImageFormats_len*sizeof(uint16_t)+sizeof(uint32_t);
	di->Manufacturer = ptp_unpack_string(params, data,
		PTP_di_OperationsSupported+totallen,
		&len);
	totallen+=len*2+1;
	di->Model = ptp_unpack_string(params, data,
		PTP_di_OperationsSupported+totallen,
		&len);
	totallen+=len*2+1;
	di->DeviceVersion = ptp_unpack_string(params, data,
		PTP_di_OperationsSupported+totallen,
		&len);
	totallen+=len*2+1;
	di->SerialNumber = ptp_unpack_string(params, data,
		PTP_di_OperationsSupported+totallen,
		&len);
}
	
/* ObjectHandles array pack/unpack */

#define PTP_oh				 0

static inline void
ptp_unpack_OH (PTPParams *params, char* data, PTPObjectHandles *oh, unsigned int len)
{
	len=0;//allocating unused variable to zero
	oh->n = ptp_unpack_uint32_t_array(params, data, PTP_oh, &oh->Handler);
}

/* StoreIDs array pack/unpack */

#define PTP_sids			 0

static inline void
ptp_unpack_SIDs (PTPParams *params, char* data, PTPStorageIDs *sids, unsigned int len)
{
	len=0;//allocating unused variable to zero
	sids->n = ptp_unpack_uint32_t_array(params, data, PTP_sids,
	&sids->Storage);
}

/* StorageInfo pack/unpack */

#define PTP_si_StorageType		 0
#define PTP_si_FilesystemType		 2
#define PTP_si_AccessCapability		 4
#define PTP_si_MaxCapability		 6
#define PTP_si_FreeSpaceInBytes		14
#define PTP_si_FreeSpaceInImages	22
#define PTP_si_StorageDescription	26

static inline void
ptp_unpack_SI (PTPParams *params, char* data, PTPStorageInfo *si, unsigned int len)
{
	uint8_t storagedescriptionlen;
	len=0; //allocating unused variable to zero
	si->StorageType=dtoh16a(&data[PTP_si_StorageType]);
	si->FilesystemType=dtoh16a(&data[PTP_si_FilesystemType]);
	si->AccessCapability=dtoh16a(&data[PTP_si_AccessCapability]);
	// modified by kskim(2007-11-23), get storage size
	si->MaxCapability=dtoh64a(&data[PTP_si_MaxCapability]);
	si->FreeSpaceInBytes=dtoh64a(&data[PTP_si_FreeSpaceInBytes]);	
	si->FreeSpaceInImages=dtoh32a(&data[PTP_si_FreeSpaceInImages]);
	si->StorageDescription=ptp_unpack_string(params, data,
		PTP_si_StorageDescription, &storagedescriptionlen);
	si->VolumeLabel=ptp_unpack_string(params, data,
		PTP_si_StorageDescription+storagedescriptionlen*2+1,
		&storagedescriptionlen);
}

/* ObjectInfo pack/unpack */

#define PTP_oi_StorageID		 0
#define PTP_oi_ObjectFormat		 4
#define PTP_oi_ProtectionStatus		 6
#define PTP_oi_ObjectCompressedSize	 8
#define PTP_oi_ThumbFormat		12
#define PTP_oi_ThumbCompressedSize	14
#define PTP_oi_ThumbPixWidth		18
#define PTP_oi_ThumbPixHeight		22
#define PTP_oi_ImagePixWidth		26
#define PTP_oi_ImagePixHeight		30
#define PTP_oi_ImageBitDepth		34
#define PTP_oi_ParentObject		38
#define PTP_oi_AssociationType		42
#define PTP_oi_AssociationDesc		44
#define PTP_oi_SequenceNumber		48
#define PTP_oi_filenamelen		52
#define PTP_oi_Filename			53

static inline uint32_t
ptp_pack_OI (PTPParams *params, PTPObjectInfo *oi, char** oidataptr)
{
	char* oidata = NULL;
	uint8_t filenamelen = 0;
	uint8_t capturedatelen=0;
	/* let's allocate some memory first; XXX i'm sure it's wrong */
	oidata=(char*)malloc(PTP_oi_Filename+(strlen(oi->Filename)+1)*2+4);
	/* the caller should free it after use! */
#if 0
	char *capture_date="20020101T010101"; /* XXX Fake date */
#endif
	if(NULL != oidata)
	{
		memset (oidata, 0, (PTP_oi_Filename+(strlen(oi->Filename)+1)*2+4));
		htod32a(&oidata[PTP_oi_StorageID],oi->StorageID);
		htod16a(&oidata[PTP_oi_ObjectFormat],oi->ObjectFormat);
		htod16a(&oidata[PTP_oi_ProtectionStatus],oi->ProtectionStatus);
		htod32a(&oidata[PTP_oi_ObjectCompressedSize],oi->ObjectCompressedSize);
		htod16a(&oidata[PTP_oi_ThumbFormat],oi->ThumbFormat);
		htod32a(&oidata[PTP_oi_ThumbCompressedSize],oi->ThumbCompressedSize);
		htod32a(&oidata[PTP_oi_ThumbPixWidth],oi->ThumbPixWidth);
		htod32a(&oidata[PTP_oi_ThumbPixHeight],oi->ThumbPixHeight);
		htod32a(&oidata[PTP_oi_ImagePixWidth],oi->ImagePixWidth);
		htod32a(&oidata[PTP_oi_ImagePixHeight],oi->ImagePixHeight);
		htod32a(&oidata[PTP_oi_ImageBitDepth],oi->ImageBitDepth);
		htod32a(&oidata[PTP_oi_ParentObject],oi->ParentObject);
		htod16a(&oidata[PTP_oi_AssociationType],oi->AssociationType);
		htod32a(&oidata[PTP_oi_AssociationDesc],oi->AssociationDesc);
		htod32a(&oidata[PTP_oi_SequenceNumber],oi->SequenceNumber);
		
		ptp_pack_string(params, oi->Filename, oidata, PTP_oi_filenamelen, &filenamelen);
	/*
		filenamelen=(uint8_t)strlen(oi->Filename);
		htod8a(&req->data[PTP_oi_filenamelen],filenamelen+1);
		for (i=0;i<filenamelen && i< PTP_MAXSTRLEN; i++) {
			req->data[PTP_oi_Filename+i*2]=oi->Filename[i];
		}
	*/
		/*
		 *XXX Fake date.
		 * for example Kodak sets Capture date on the basis of EXIF data.
		 * Spec says that this field is from perspective of Initiator.
		 */
#if 0	/* seems now we don't need any data packed in OI dataset... for now ;)*/
		capturedatelen=strlen(capture_date);
		htod8a(&data[PTP_oi_Filename+(filenamelen+1)*2],
			capturedatelen+1);
		for (i=0;i<capturedatelen && i< PTP_MAXSTRLEN; i++) {
			data[PTP_oi_Filename+(i+filenamelen+1)*2+1]=capture_date[i];
		}
		htod8a(&data[PTP_oi_Filename+(filenamelen+capturedatelen+2)*2+1],
			capturedatelen+1);
		for (i=0;i<capturedatelen && i< PTP_MAXSTRLEN; i++) {
			data[PTP_oi_Filename+(i+filenamelen+capturedatelen+2)*2+2]=
			  capture_date[i];
		}
#endif
		/* XXX this function should return dataset length */
		*oidataptr=oidata;
	}
	
	return (PTP_oi_Filename+(filenamelen+1)*2+(capturedatelen+1)*4);
}

static inline void
ptp_unpack_OI (PTPParams *params, char* data, PTPObjectInfo *oi, unsigned int len)
{
	uint8_t filenamelen;
	uint8_t capturedatelen;
	char *capture_date;
	char tmp[16];
	struct tm tm;
	len =len;

	memset(&tm,0,sizeof(tm));

	oi->StorageID=dtoh32a(&data[PTP_oi_StorageID]);
	oi->ObjectFormat=dtoh16a(&data[PTP_oi_ObjectFormat]);
	oi->ProtectionStatus=dtoh16a(&data[PTP_oi_ProtectionStatus]);
	oi->ObjectCompressedSize=dtoh32a(&data[PTP_oi_ObjectCompressedSize]);
	oi->ThumbFormat=dtoh16a(&data[PTP_oi_ThumbFormat]);
	oi->ThumbCompressedSize=dtoh32a(&data[PTP_oi_ThumbCompressedSize]);
	oi->ThumbPixWidth=dtoh32a(&data[PTP_oi_ThumbPixWidth]);
	oi->ThumbPixHeight=dtoh32a(&data[PTP_oi_ThumbPixHeight]);
	oi->ImagePixWidth=dtoh32a(&data[PTP_oi_ImagePixWidth]);
	oi->ImagePixHeight=dtoh32a(&data[PTP_oi_ImagePixHeight]);
	oi->ImageBitDepth=dtoh32a(&data[PTP_oi_ImageBitDepth]);
	oi->ParentObject=dtoh32a(&data[PTP_oi_ParentObject]);
	oi->AssociationType=dtoh16a(&data[PTP_oi_AssociationType]);
	oi->AssociationDesc=dtoh32a(&data[PTP_oi_AssociationDesc]);
	oi->SequenceNumber=dtoh32a(&data[PTP_oi_SequenceNumber]);
	oi->Filename= ptp_unpack_string(params, data, PTP_oi_filenamelen, &filenamelen);

	capture_date = ptp_unpack_string(params, data,
		PTP_oi_filenamelen+filenamelen*2+1, &capturedatelen);
	/* subset of ISO 8601, without '.s' tenths of second and 
	 * time zone
	 */
	if ((NULL != capture_date)&&(capturedatelen>15))
	{
		strncpy (tmp, capture_date, 4);
		tmp[4] = 0;
		tm.tm_year=atoi (tmp) - 1900;
		strncpy (tmp, capture_date + 4, 2);
		tmp[2] = 0;
		tm.tm_mon = atoi (tmp) - 1;
		strncpy (tmp, capture_date + 6, 2);
		tmp[2] = 0;
		tm.tm_mday = atoi (tmp);
		strncpy (tmp, capture_date + 9, 2);
		tmp[2] = 0;
		tm.tm_hour = atoi (tmp);
		strncpy (tmp, capture_date + 11, 2);
		tmp[2] = 0;
		tm.tm_min = atoi (tmp);
		strncpy (tmp, capture_date + 13, 2);
		tmp[2] = 0;
		tm.tm_sec = atoi (tmp);
		oi->CaptureDate=mktime (&tm);
	}
	PTPFreeMem(capture_date);

	/* now it's modification date ;) */
	capture_date = ptp_unpack_string(params, data,
		PTP_oi_filenamelen+filenamelen*2
		+capturedatelen*2+2,&capturedatelen);
	if ((NULL != capture_date)&&(capturedatelen>15))
	{
		strncpy (tmp, capture_date, 4);
		tmp[4] = 0;
		tm.tm_year=atoi (tmp) - 1900;
		strncpy (tmp, capture_date + 4, 2);
		tmp[2] = 0;
		tm.tm_mon = atoi (tmp) - 1;
		strncpy (tmp, capture_date + 6, 2);
		tmp[2] = 0;
		tm.tm_mday = atoi (tmp);
		strncpy (tmp, capture_date + 9, 2);
		tmp[2] = 0;
		tm.tm_hour = atoi (tmp);
		strncpy (tmp, capture_date + 11, 2);
		tmp[2] = 0;
		tm.tm_min = atoi (tmp);
		strncpy (tmp, capture_date + 13, 2);
		tmp[2] = 0;
		tm.tm_sec = atoi (tmp);
		oi->ModificationDate=mktime (&tm);
	}
	PTPFreeMem(capture_date);
}




/* Custom Type Value Assignement (without Length) macro frequently used below */
#define CTVAL(target,func) {			\
	if (total - *offset < sizeof(target))	\
		return 0;			\
	target = func(&data[*offset]);		\
	*offset += sizeof(target);		\
}

#define RARR(val,member,func) {			\
	int n,j;					\
	if (total - *offset < sizeof(uint32_t))		\
		return 0;				\
	n = dtoh32a (&data[*offset]);			\
	*offset += sizeof(uint32_t);			\
							\
	val->a.count = n;				\
	val->a.v = (PTPPropertyValue*)malloc(sizeof(val->a.v[0])*n);	\
	if (!val->a.v) return 0; \
	for (j=0;j<n;j++)				\
		CTVAL(val->a.v[j].member, func);	\
}


//void RARR(PTPPropertyValue* value,int8_t i8,)

static inline int ptp_unpack_DPV (PTPParams *params, unsigned char* data, int *offset, /*int*/ unsigned int total,	PTPPropertyValue* value, uint16_t datatype)
 {

	switch (datatype) {
		case PTP_DTC_INT8:
			CTVAL(value->i8,dtoh8a);
			break;
		case PTP_DTC_UINT8:
			CTVAL(value->u8,dtoh8a);
			break;
		case PTP_DTC_INT16:
			CTVAL(value->i16,dtoh16a);
			break;
		case PTP_DTC_UINT16:
			CTVAL(value->u16,dtoh16a);
			break;
		case PTP_DTC_INT32:
			CTVAL(value->i32,dtoh32a);
			break;
		case PTP_DTC_UINT32:
			CTVAL(value->u32,dtoh32a);
			break;
		case PTP_DTC_AINT8:
			RARR(value,i8,dtoh8a);
			break;
		case PTP_DTC_AUINT8:
			RARR(value,u8,dtoh8a);
			break;
		case PTP_DTC_AUINT16:
			RARR(value,u16,dtoh16a);
			break;
		case PTP_DTC_AINT16:
			RARR(value,i16,dtoh16a);
			break;
		case PTP_DTC_AUINT32:
			RARR(value,u32,dtoh32a);
			break;
		case PTP_DTC_AINT32:
			RARR(value,i32,dtoh32a);
			break;
		/* XXX: other int types are unimplemented */
		/* XXX: other int arrays are unimplemented also */
		case PTP_DTC_STR: {
			uint8_t len;
			/* XXX: max size */
			value->str = ptp_unpack_string(params,(char*)data,*offset,&len);
			*offset += len*2+1;
			if (!value->str)
				return 0;
			break;
		}
		default:
			return 0;
	}

	return 1;
}

/* Device Property pack/unpack */

#define PTP_dpd_DevicePropertyCode	0
#define PTP_dpd_DataType		2
#define PTP_dpd_GetSet			4
#define PTP_dpd_FactoryDefaultValue	5

static inline int
ptp_unpack_DPD (PTPParams *params, char* data, PTPDevicePropDesc *dpd, unsigned int dpdlen)
{
	int offset=0, ret;

	memset (dpd, 0, sizeof(*dpd));
	dpd->DevicePropertyCode=dtoh16a(&data[PTP_dpd_DevicePropertyCode]);
	dpd->DataType=dtoh16a(&data[PTP_dpd_DataType]);
	dpd->GetSet=dtoh8a(&data[PTP_dpd_GetSet]);

	offset = PTP_dpd_FactoryDefaultValue;
	ret = ptp_unpack_DPV (params, (unsigned char*)data, &offset, dpdlen, &dpd->FactoryDefaultValue, dpd->DataType);//g++ typecast vishal
	if (!ret) goto outofmemory;
	ret = ptp_unpack_DPV (params, (unsigned char*)data, &offset, dpdlen, &dpd->CurrentValue, dpd->DataType);//g++ typecast vishal
	if (!ret) goto outofmemory;

	/* if offset==0 then Data Type format is not supported by this
	   code or the Data Type is a string (with two empty strings as
	   values). In both cases Form Flag should be set to 0x00 and FORM is
	   not present. */

	dpd->FormFlag=PTP_DPFF_None;
	if (offset==PTP_dpd_FactoryDefaultValue)
		return 1;

	dpd->FormFlag=dtoh8a(&data[offset]);
	offset+=sizeof(uint8_t);

	switch (dpd->FormFlag) {
		case PTP_DPFF_Range:
			ret = ptp_unpack_DPV (params, (unsigned char*)data, &offset, dpdlen, &dpd->FORM.Range.MinimumValue, dpd->DataType);//g++ typecast vishal
			if (!ret) goto outofmemory;
			ret = ptp_unpack_DPV (params, (unsigned char*)data, &offset, dpdlen, &dpd->FORM.Range.MaximumValue, dpd->DataType);//g++ typecast vishal
			if (!ret) goto outofmemory;
			ret = ptp_unpack_DPV (params, (unsigned char*)data, &offset, dpdlen, &dpd->FORM.Range.StepSize, dpd->DataType);//g++ typecast vishal
			if (!ret) goto outofmemory;
			break;
		case PTP_DPFF_Enumeration: {
			int i;
#define N	dpd->FORM.Enum.NumberOfValues
			N = dtoh16a(&data[offset]);
			offset+=sizeof(uint16_t);
			dpd->FORM.Enum.SupportedValue = (PTPPropertyValue*)malloc(N*sizeof(dpd->FORM.Enum.SupportedValue[0]));//g++ typecast vishal
			if (!dpd->FORM.Enum.SupportedValue)
				goto outofmemory;

			memset (dpd->FORM.Enum.SupportedValue,0 , N*sizeof(dpd->FORM.Enum.SupportedValue[0]));
			for (i=0;i<N;i++) {
				ret = ptp_unpack_DPV (params, (unsigned char*)data, &offset, dpdlen, &dpd->FORM.Enum.SupportedValue[i], dpd->DataType);//g++ typecast vishal

				/* Slightly different handling here. The HP PhotoSmart 120
				 * specifies an enumeration with N in wrong endian
				 * 00 01 instead of 01 00, so we count the enum just until the
				 * the end of the packet.
				 */
				if (!ret) {
					if (!i)
						goto outofmemory;
					dpd->FORM.Enum.NumberOfValues = i;
					break;
				}
			}
			}
		default:
			goto outofmemory;;
	}
	return 1;
outofmemory:
	ptp_free_devicepropdesc(dpd);
	return 0;
}

static inline uint32_t
ptp_pack_DPV (PTPParams *params, PTPPropertyValue* value, char** dpvptr, uint16_t datatype)
{
	char* dpv=NULL;
	uint32_t size=0;
	uint32_t i;
	//int	i;

	switch (datatype) {
		case PTP_DTC_INT8:
			size=sizeof(int8_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod8a(dpv,value->i8);
			}

			break;
		case PTP_DTC_UINT8:
			size=sizeof(uint8_t);
			dpv=(char*)malloc(size);	//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod8a(dpv,value->u8);
			}
			break;
		case PTP_DTC_INT16:
			size=sizeof(int16_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod16a(dpv,value->i16);
			}
			break;
		case PTP_DTC_UINT16:	
			size=sizeof(uint16_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod16a(dpv,value->u16);
			}
			break;
		case PTP_DTC_INT32:
			size=sizeof(int32_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->i32);
			}
			break;
		case PTP_DTC_UINT32:
			size=sizeof(uint32_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->u32);
			}
			break;
		case PTP_DTC_AUINT8:
			size=sizeof(uint32_t)+value->a.count*sizeof(uint8_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->a.count);
				for (i=0;i<value->a.count;i++)
					htod8a(&dpv[4+i],value->a.v[i].u8);
			}
			break;
		case PTP_DTC_AINT8:
			size=sizeof(uint32_t)+value->a.count*sizeof(int8_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->a.count);
				for (i=0;i<value->a.count;i++)
					htod8a(&dpv[4+i],value->a.v[i].i8);
			}
			break;
		case PTP_DTC_AUINT16:
			size=sizeof(uint32_t)+value->a.count*sizeof(uint16_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->a.count);
				for (i=0;i<value->a.count;i++)
					htod16a(&dpv[4+i],value->a.v[i].u16);
			}
			break;
		case PTP_DTC_AINT16:
			size=sizeof(uint32_t)+value->a.count*sizeof(int16_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->a.count);
				for (i=0;i<value->a.count;i++)
					htod16a(&dpv[4+i],value->a.v[i].i16);
			}
			break;
		case PTP_DTC_AUINT32:
			size=sizeof(uint32_t)+value->a.count*sizeof(uint32_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->a.count);
				for (i=0;i<value->a.count;i++)
					htod32a(&dpv[4+i],value->a.v[i].u32);
			}
			break;
		case PTP_DTC_AINT32:
			size=sizeof(uint32_t)+value->a.count*sizeof(int32_t);
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				htod32a(dpv,value->a.count);
				for (i=0;i<value->a.count;i++)
					htod32a(&dpv[4+i],value->a.v[i].i32);
			}
			break;
		/* XXX: other int types are unimplemented */
		case PTP_DTC_STR: {
			uint8_t len;

			size=strlen(value->str)*2+3;
			dpv=(char*)malloc(size);//g++ typecast vishal
			if(NULL != dpv)
			{
				memset(dpv,0,size);
				ptp_pack_string(params, value->str, dpv, 0, &len);
			}
			break;
		}
		default:
			return 0;
	}
	*dpvptr=dpv;
	return size;
}

/*
    PTP USB Event container unpack
    Copyright (c) 2003 Nikolai Kopanygin
*/

#define PTP_ec_Length		0
#define PTP_ec_Type		4
#define PTP_ec_Code		6
#define PTP_ec_TransId		8
#define PTP_ec_Param1		12
#define PTP_ec_Param2		16
#define PTP_ec_Param3		20

static inline void
ptp_unpack_EC (PTPParams *params, char* data, PTPUSBEventContainer *ec, unsigned int len)
{
	if (data==NULL)
		return;
	len = 0; //intialising unused argument
	ec->length=dtoh32a(&data[PTP_ec_Length]);
	ec->type=dtoh16a(&data[PTP_ec_Type]);
	ec->code=dtoh16a(&data[PTP_ec_Code]);
	ec->trans_id=dtoh32a(&data[PTP_ec_TransId]);

	if (ec->length>=(PTP_ec_Param1+4))
		ec->param1=dtoh32a(&data[PTP_ec_Param1]);
	else
		ec->param1=0;
	if (ec->length>=(PTP_ec_Param2+4))
		ec->param2=dtoh32a(&data[PTP_ec_Param2]);
	else
		ec->param2=0;
	if (ec->length>=(PTP_ec_Param3+4))
		ec->param3=dtoh32a(&data[PTP_ec_Param3]);
	else
		ec->param3=0;
}

/*
    PTP Canon Folder Entry unpack
    Copyright (c) 2003 Nikolai Kopanygin
*/
#define PTP_cfe_ObjectHandle		0
#define PTP_cfe_ObjectFormatCode	4
#define PTP_cfe_Flags			6
#define PTP_cfe_ObjectSize		7
#define PTP_cfe_Time			11
#define PTP_cfe_Filename		15

static inline void
ptp_unpack_Canon_FE (PTPParams *params, char* data, PTPCANONFolderEntry *fe)
{
	int i;
	if (data==NULL)
		return;
	fe->ObjectHandle=dtoh32a(&data[PTP_cfe_ObjectHandle]);
	fe->ObjectFormatCode=dtoh16a(&data[PTP_cfe_ObjectFormatCode]);
	fe->Flags=dtoh8a(&data[PTP_cfe_Flags]);
	fe->ObjectSize=dtoh32a(&data[PTP_cfe_ObjectSize]);
	fe->Time=(time_t)dtoh32a(&data[PTP_cfe_Time]);
	for (i=0; i<PTP_CANON_FilenameBufferLen; i++)
	fe->Filename[i]=(char)dtoh8a(&data[PTP_cfe_Filename+i]);
}


