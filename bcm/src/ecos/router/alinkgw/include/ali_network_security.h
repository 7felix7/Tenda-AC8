/*
 * Copyright (c) 2014-2015 Alibaba Group. All rights reserved.
 *
 * Alibaba Group retains all right, title and interest (including all
 * intellectual property rights) in and to this computer program, which is
 * protected by applicable intellectual property laws.  Unless you have 
 * obtained a separate written license from Alibaba Group., you are not 
 * authorized to utilize all or a part of this computer program for any 
 * purpose (including reproduction, distribution, modification, and 
 * compilation into object code), and you must immediately destroy or 
 * return to Alibaba Group all copies of this computer program.  If you 
 * are licensed by Alibaba Group, your rights to utilize this computer 
 * program are limited by the terms of that license.  To obtain a license, 
 * please contact Alibaba Group.
 *
 * This computer program contains trade secrets owned by Alibaba Group.
 * and, unless unauthorized by Alibaba Group in writing, you agree to
 * maintain the confidentiality of this computer program and related 
 * information and to not disclose this computer program and related 
 * information to any other person or entity.
 *
 * THIS COMPUTER PROGRAM IS PROVIDED AS IS WITHOUT ANY WARRANTIES, AND 
 * Alibaba Group EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
 * INCLUDING THE WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE, AND NONINFRINGEMENT.
 */

#ifndef __ALI_NETWORK_SECURITY_H__
#define __ALI_NETWORK_SECURITY_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ALI_NS_OK    0
#define ALI_NS_ERR  -1

/*
 * @desc����ȫ�����ö�١���Ҫ�ر�˵�����ǣ�
 *    1. ALI_NS_RESULT_unavailable������������ͻ��⣬���������ΪALI_NS_RESULT_unavailable��
 *    ����ͬʱ���������κ����͡�
 *    2. ALI_NS_RESULT_normal��ALI_NS_RESULT_phishing���߻��⣬�����������ͬʱ���������ߡ�
 *    3. ALI_NS_RESULT_hijacking��ALI_NS_RESULT_normal/ALI_NS_RESULT_phishing�ɹ��棬
 *    ����������ԼȰ���ALI_NS_RESULT_hijacking�ְ����������е�һ����
 */
typedef enum
{
    ALI_NS_RESULT_BIT_unavailable = 0,   // �����ﰲȫ��������δ����
    ALI_NS_RESULT_BIT_normal,            // ��������
    ALI_NS_RESULT_BIT_phishing,          // ����Ϊ������վ
    ALI_NS_RESULT_BIT_hijacking          // DNS�ٳ�
}ALI_NS_RESULT_BIT_E;

/*
 * @desc����ȫ����Ӧ�������
 */
#define ALI_NS_RESULT_MASK_unavailable (1 << ALI_NS_RESULT_BIT_unavailable)
#define ALI_NS_RESULT_MASK_normal      (1 << ALI_NS_RESULT_BIT_normal)
#define ALI_NS_RESULT_MASK_phishing   (1 << ALI_NS_RESULT_BIT_phishing)
#define ALI_NS_RESULT_MASK_hijacking   (1 << ALI_NS_RESULT_BIT_hijacking)


/*************************************************************************
Function:      int ali_ns_open(void);
Description:   ���������ﰲȫ������
Input:         ��
Output:        ��
Return:        ALI_NS_OK: �ɹ�
               ����:        ʧ��
Others:
*************************************************************************/
extern int ali_ns_open(void);


/*************************************************************************
Function:      int ali_ns_close(void);
Description:   ֹͣ�����ﰲȫ������
Input:         ��
Output:        ��
Return:        ALI_NS_OK: �ɹ�
               ����:        ʧ��
Others:
*************************************************************************/
extern int ali_ns_close(void);


/*************************************************************************
Function:      int ali_ns_update(void);
Description:   ���¡����ﰲȫ������İ�ȫ��
Input:         ��
Output:        ��
Return:        ALI_NS_OK: ���³ɹ�
               ����:        ����ʧ��
Others:
*************************************************************************/
extern int ali_ns_update(void);


/*************************************************************************
Description:   �ƶ���в��ѯ����ص��������͡�
Input:         domain��     ��������'\0'������
               mac_addr�� �����豸��MAC��ַ
               dns_srv_ip��DNS������IP��ַ��
Output:        result��   ���ﰲȫ���������������ȡֵ���ܣ�
                 ALI_NS_RESULT_MASK_unavailable�� �����ﰲȫ������δ����
                 ALI_NS_RESULT_MASK_normal:      ��������
                 ALI_NS_RESULT_MASK_phishing:    ���е�����վ
                 ALI_NS_RESULT_MASK_hijacking:   ����DNS�ٳ�
                 ALI_NS_RESULT_MASK_phishing | ALI_NS_RESULT_MASK_hijacking: 
                  ���ʵ�����վ�ұ�DNS�ٳ�
Return:        ALI_NS_OK�� ���ɹ������������μ�result������
               ������      ������
���¸ô���       
Others:
*************************************************************************/
typedef void (*ali_ns_reply_callback)(unsigned int query_flag,
                                      const char *domain, 
                                      const unsigned char *mac, 
                                      unsigned int dns_srv_ip, 
                                      unsigned int result);


/*************************************************************************
Function:      int ali_ns_set_reply_cb(ali_ns_reply_callback cb);
Description:   �����ƶ���в��ѯ����Ļص�������
Input:         host��     �����������NULL,��Ҫ����'\0'������
Output:        result��   ���ﰲȫ���������������ȡֵ���ܣ�
Return:        ALI_NS_OK�� ���ɹ������������μ�result������
               ������      ������
���¸ô���       
Others:
*************************************************************************/
extern int ali_ns_set_reply_cb(ali_ns_reply_callback cb);


/*************************************************************************
Function:      int ali_ns_query(const char *domain,
                         unsigned char mac_addr[6],
                         unsigned int dns_srv_ip);
Description:   ����Ƿ����а��������ߵ�����վ����Ҫ˵�����ǣ�������Ϊ����
               ������ʹ���ʵ��ǵ�����վ���ú����Է���   
               ALI_NS_PHISHING_WHITELIST����
Input:         domain��     �����������NULL,��Ҫ����'\0'������
               mac_addr�� �����豸��MAC��ַ������ǿգ����ڼ���Ƿ�����
                           ��������
               dns_srv_ip��DNS������IP��ַ��
Output:        
Return:        ALI_NS_OK�� ���ɹ������������μ�result������
               ������      ������
���¸ô���       
Others:
*************************************************************************/
extern int ali_ns_query(
        unsigned int query_flag,
        const char *domain, 
        unsigned char mac_addr[6], 
        unsigned int dns_srv_ip);


/*************************************************************************
Function:      const char* ali_ns_get_version(void)
Description:   ��ȡ�����ﰲȫ�����԰汾��
Input:         ��
Output:        ��
Return:        �汾�ַ���
Others:
*************************************************************************/
extern const char* ali_ns_get_version(void);


#ifdef __cplusplus
}
#endif

#endif // __ALI_NETWORK_SECURITY_H__

