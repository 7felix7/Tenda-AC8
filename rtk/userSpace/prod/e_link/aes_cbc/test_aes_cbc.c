#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "aes.h"



//g++ -g -o -Wall -m64 AesTest AesTest.cpp -lssl -lcrypto
//g++ -g -o -Wall AesTest AesTest.cpp -lssl -lcrypto

int aes_cbc_test(int argc, char **argv)
{//������ֱ�ӶԽ��õ�char����ô�ӽ���Ҫǿ��ת��
	char Source[1024];
	char *InputData=NULL;
	char *EncryptData=NULL;
	char *DecryptData=NULL;
	
	unsigned char Key[AES_BLOCK_SIZE+1];	//������unsigned char
	unsigned char ivec[AES_BLOCK_SIZE];		//������unsigned char
	AES_KEY AesKey;
	
	int DataLen=0,SetDataLen=0, i;

	memset(Source, 0x00, sizeof(Source));
	strcpy(Source, "1234567890abcdeasdfasdfasdfasdfasdfasdfasd");	//Ҫ���ܵ�����
	DataLen = strlen(Source);

	memset(Key, 0x00, sizeof(Key));
	memcpy(Key, "0123456789abcdefss", AES_BLOCK_SIZE);

 // set the encryption length
    SetDataLen = 0;
	if ((DataLen%AES_BLOCK_SIZE) == 0)
	{
        SetDataLen = DataLen;
    }
    else
    {
        SetDataLen = ((DataLen/AES_BLOCK_SIZE)+1) * AES_BLOCK_SIZE;
    }
    printf("--SetDataLen:%d...\n", SetDataLen);	//ȡ16�ı���
	
	InputData = (char *)calloc(SetDataLen+1, sizeof(char));
    if(InputData == NULL)	//ע��ҪSetDataLen+1
    {
        fprintf(stderr, "Unable to allocate memory for InputData\n");
        exit(-1);
    }
    memcpy(InputData, Source, DataLen);
	
	EncryptData = (char *)calloc(SetDataLen+1, sizeof(char));
    if(EncryptData == NULL)	//ע��ҪSetDataLen+1
    {
        fprintf(stderr, "Unable to allocate memory for EncryptData\n");
        exit(-1);
    }
    
    DecryptData = (char *)calloc(SetDataLen+1, sizeof(char));
    if(DecryptData == NULL)	//ע��ҪSetDataLen+1
    {
        fprintf(stderr, "Unable to allocate memory for DecryptData\n");
        exit(-1);
    }

	memset(&AesKey, 0x00, sizeof(AES_KEY));
	if(AES_set_encrypt_key(Key, 128, &AesKey) < 0)
	{//���ü�����Կ
		fprintf(stderr, "Unable to set encryption key in AES...\n");
		exit(-1);
	}
   

	for(i=0; i<AES_BLOCK_SIZE; i++)
	{//����Ҫ��
		ivec[i] = 0;
	}
	//����
	AES_cbc_encrypt((unsigned char *)InputData, (unsigned char *)EncryptData, 
		SetDataLen, &AesKey, ivec, AES_ENCRYPT);   

	memset(&AesKey, 0x00, sizeof(AES_KEY));
	if(AES_set_decrypt_key(Key, 128, &AesKey) < 0)
	{//���ý�����Կ
		fprintf(stderr, "Unable to set encryption key in AES...\n");
		exit(-1);
	}

	for(i=0; i<AES_BLOCK_SIZE; i++)
	{//����Ҫ��
		ivec[i] = 0;
	}
	//����
	AES_cbc_encrypt((unsigned char *)EncryptData, (unsigned char *)DecryptData, 
		SetDataLen, &AesKey, ivec, AES_DECRYPT); 

	printf("DecryptData:%s...\n", (char *)DecryptData);

	if(InputData != NULL)
	{
		free(InputData);
		InputData = NULL;
	}
	
	if(EncryptData != NULL)
	{
		free(EncryptData);
		EncryptData = NULL;
	}
	
	if(DecryptData != NULL)
	{
		free(DecryptData);
		DecryptData = NULL;
	}

	return 0;
}

