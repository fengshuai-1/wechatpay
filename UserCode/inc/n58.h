#ifndef __N58_H__
#define __N58_H__



void N58_init(void); //N58��ʼ��
void Clear_Buffer(void);//��ջ���
void Clear_txbuffer(void);//���txbuffer����
void AT_check(void);    //���ATָ��
void CSQ_check(void);   //���ģ���ź�
void IP_allo(void);    //����ip
void HTTPSCFG_conect(void); //ֱ��������ģʽ
void HTTPS_cfg(void); //ֱ��������ģʽ
void HTTPS_para(void);   //����΢��֧���˿�
void Conect_HTTPS(void);   // ��΢��֧���ӿڽ�������
//void Send_HTTPS(void);    // ��΢��֧���ӿڷ���xml
void HTTPS_check(void);   //����΢�Ų�ѯ�˿�
extern uint16_t Rxcouter;
#endif
