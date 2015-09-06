/*
struct ncSflow_s {
        unsigned long lService;
        unsigned long lGroupid;
        unsigned long long lBytes;
        unsigned long long lBytesqs[32];      //用于趋势图，按统计时间顺序存放
        char caGname[64];
}; 
*/
struct piechart_datasetf{
	char *name;                               //饼图项目名称
	char **append;                            //pdf表格中附加字段，从第三列开始
	//unsigned short  len;
	float amount;                             //项目数据
	char *percent;                            //百分数
};
struct chart_datasetf {
	char *name;                               //时间
	float *dataset;                           //时间点数据
};
/*
   功能                            输出饼图
   int iFd                         PAS里面的 iFd, 后面用其调用了utPltFileDownload下载函数
   struct piechart_datasetf *chart 数据
   char **desc                     表头 如{"用户名", "部门", "流量(KB)", "百分比"}
   int rowrow                      chart 长度
   int column                      desc  长度
   int applen                      附加字段个数
   char *title                     标题
   char *unit                      单位标注 如 "单位(KB)"
   char *filename_new              PDF文件名
   char *report_text               统计时间标识

 */
int ncWebShowServiceClassFirst_PDF(int iFd, struct piechart_datasetf *chart, char **desc, int rowrow, int column, int applen, char *title, char *unit, char *report_text, char *filename_new);  


 /*
   功能                            趋势图
   int iFd                         PAS里面的 iFd, 后面用其调用了utPltFileDownload下载函数
   struct piechart_datasetf *chart 数据
   char **desc                     表头
   int rowrow                      chart 长度
   int column                      desc  长度
   char *title                     标题
   char *unit                      单位标注 如 "单位(KB)"
   char *filename_new              PDF文件名
*/
int ncWebShowServiceBytime_PDF(int iFd, struct chart_datasetf *chart, char **desc, int rowrow, int column, char *title, char *unit, char *report_text, char *filename_new);