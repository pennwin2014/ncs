#ifndef UTOBILL_DEF
#define UTOBILL_DEF
typedef struct {
    unsigned long    lLow;      /* 下限     */   
    unsigned long    lUp;       /* 上限     */
    double           dBase;     /* 基本费用 */
    double           dRate;     /* 费率     */
    unsigned         lMode;     /* 计费方式 0 -- 按时间   1---按字节*/
} utBilBaseFee;

typedef struct utBilDayDiscount_s {
   unsigned long     lStart;    /* 起始日期 */
   unsigned long     lStop;     /* 终止日期 */
   double            dDiscount; /* 折扣     */
   struct utBilDayDiscount_s  *psNext;
} utBilDayDiscount;

typedef struct utBilWeekDiscount_s {
    unsigned long      lWeekDay;     /* 星期  1--7 */
    double             dDisCount;    /* 折扣       */
    struct utBilWeekDiscount_s  *psNext;
} utBilWeekDiscount;


typedef struct utBilTimeDiscount_s {
    unsigned long      lStart;       /* 起始时间  */
    unsigned long      lStop;        /* 终止时间  */
    double            dDiscount;     /* 折扣      */
    struct utBilTimeDiscount_s   *psNext;
} utBilTimeDiscount;


typedef  struct utBilDiscount_s{
    unsigned long    lStart;         /* 起始时间  */
    unsigned long    lEnd;           /* 终止时间  */
    double           dDiscount;      /* 折扣      */
    struct utBilDiscount_s   *psNext;
} utBilDiscountModule;


typedef  struct utBilFeeTable_s {
    long                              lRoamFlags;
    utBilBaseFee                     *psBilBaseFee;   
    utBilDayDiscount                 *psBilDayDiscount;
    utBilWeekDiscount                *psBilWeekDiscount;
    utBilTimeDiscount                *psBilTimeDiscount;
    utBilDiscountModule              *psBilDiscountModule;
    struct utBilFeeTable_s           *psNext;
}  utBilFeeTable;



typedef struct utBilClass_s {
     unsigned   short      nClass;
     utBilFeeTable         *psBilFeeTable;
     struct utBilClass_s   *psNext;
} utBilClass;

typedef struct utBilAccess_s {
      unsigned short  nAccess;    /* Access Type */
      utBilClass      *psBilClass;
      struct utBilAccess_s   *psNext;
} utBilAccess;


typedef struct utBilNode_s {
      char            caNodeName[16];
      utBilAccess    *psBilAccess;
      struct utBilNode_s   *psNextNode;
}  utBilNode;

typedef struct {
      utBilNode     *psBilNode;
      long	lStart;
      long	lEnd;
} utBilFeeRate;

double  utBilComputeFee(utBilFeeRate *psBilFeeRate,
        	           char *pNodeName, 
                       unsigned short  nClass,
                       unsigned short  nAccess,
                       long lUsedTime, long lStartTime,
                       long lSumTime,  long *lThisTime);
                       
utBilFeeRate  *utBilLoadFeeTable(char *pFeeName);
long   utBilLeftTime(utBilFeeRate *psBilFeeRate,
	           char *pNodeName, 
               unsigned short  nClass,
               unsigned short  nAccess,
               double dLeftFee,long lUsedTime,long lStartTime);
void utBilFreeFeeTable(utBilFeeRate *psBilFeeRate);
int utBilGenFeeModule(utBilFeeRate *psBilFeeRate,long lStart,long lStop);

#endif
