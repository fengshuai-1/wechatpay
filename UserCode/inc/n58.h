#ifndef __N58_H__
#define __N58_H__



void N58_init(void); //N58初始化
void Clear_Buffer(void);//清空缓存
void Clear_txbuffer(void);//清空txbuffer缓存
void AT_check(void);    //检查AT指令
void CSQ_check(void);   //检查模块信号
void IP_allo(void);    //分配ip
void HTTPSCFG_conect(void); //直连不加密模式
void HTTPS_cfg(void); //直连不加密模式
void HTTPS_para(void);   //连接微信支付端口
void Conect_HTTPS(void);   // 与微信支付接口建立连接
//void Send_HTTPS(void);    // 向微信支付接口发送xml
void HTTPS_check(void);   //连接微信查询端口
extern uint16_t Rxcouter;
#endif
