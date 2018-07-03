#include <pkgconf/system.h>
#include <pkgconf/devs_eth_rltk_819x_wlan.h>
#include <cyg/hal/bspchip.h>
#include <cyg/infra/diag.h>
#include <cyg/kernel/kapi.h>
#include "gpio_api.h"

#define RTL_R32(addr)       (*(volatile unsigned long *)(addr))
#define RTL_W32(addr, l)    ((*(volatile unsigned long*)(addr)) = (l))

/*****************************************************************************
 �� �� ��  : rtl819x_gpio_mux
 ��������  : ��ȡ gpio �ĸ���ֵ
 �������  : pin
 �������  : value , address
 �� �� ֵ  : make
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��15��
    ��    ��   : ��־
    �޸�����   : �����ɺ���
    �޸��ˣ� ��־ ����v2.0�ӿ�

*****************************************************************************/

static unsigned int rtl819x_gpio_mux(unsigned int pin, 
    unsigned int *value, unsigned int *address )
{
    unsigned int mask = 0;

    switch(pin) {
    case BSP_GPIO_PIN_A0:
        mask = 0xf<<28;
        *value = 0x8<<28;
        *address = BSP_PIN_MUX_SEL7;
        break;
    case BSP_GPIO_PIN_A1:
        mask = 0xf<<28;
        *value = 0x8<<28;
        *address = BSP_PIN_MUX_SEL6;
        break;
    case BSP_GPIO_PIN_A2:
        mask = 0xf<<24;
        *value = 0xb<<24;
        *address = BSP_PIN_MUX_SEL6;
        break;
    case BSP_GPIO_PIN_A3:
        mask = 0xf<<16;
        *value = 0x8<<16;
        *address = BSP_PIN_MUX_SEL7;
        break;
    case BSP_GPIO_PIN_A4:
        mask = 0xf<<24;
        *value = 0x7<<24;
        *address = BSP_PIN_MUX_SEL7;
        break;
    case BSP_GPIO_PIN_A5:
        mask = 0xf<<20;
        *value = 0x6<<20;
        *address = BSP_PIN_MUX_SEL7;
        break;
    case BSP_GPIO_PIN_A6:
        mask = 0xf<<16;
        *value = 0x5<<16;
        *address = BSP_PIN_MUX_SEL0;
        break;
    case BSP_GPIO_PIN_A7:
        mask = 0xf<<20;
        *value = 0x6<<20;
        *address = BSP_PIN_MUX_SEL0;
        break;

    case BSP_GPIO_PIN_B0:
        mask = 0xf<<24;
        *value = 0x8<<24;
        *address = BSP_PIN_MUX_SEL0;
        break;
    case BSP_GPIO_PIN_B1:
        mask = 0xf<<16;
        *value = 0xa<<16;
        *address = BSP_PIN_MUX_SEL2;
        break;
    case BSP_GPIO_PIN_B2:
        mask = 0xf<<16;
        *value = 0x8<<16;
        *address = BSP_PIN_MUX_SEL1;
        break;
    case BSP_GPIO_PIN_B3:
        mask = 0xf<<20;
        *value = 0x8<<20;
        *address = BSP_PIN_MUX_SEL1;
        break;
    case BSP_GPIO_PIN_B4:
        mask = 0xf<<24;
        *value = 0x8<<24;
        *address = BSP_PIN_MUX_SEL1;
        break;
    case BSP_GPIO_PIN_B5:
        mask = 0xf<<28;
        *value = 0x7<<28;
        *address = BSP_PIN_MUX_SEL1;
        break;
    case BSP_GPIO_PIN_B6:
        mask = 0xf<<28;
        *value = 0x8<<28;
        *address = BSP_PIN_MUX_SEL0;
        break;
    case BSP_GPIO_PIN_B7:
        mask = 0xf<<24;
        *value = 0x8<<24;
        *address = BSP_PIN_MUX_SEL2;
        break;

    case BSP_GPIO_PIN_C0:
        mask = 0xf<<20;
        *value = 0x6<<20;
        *address = BSP_PIN_MUX_SEL2;
        break;
    case BSP_GPIO_PIN_C1:
        mask = 0xf<<12;
        *value = 0x7<<12;
        *address = BSP_PIN_MUX_SEL2;
        break;
    case BSP_GPIO_PIN_C2:
        mask = 0xf<<8;
        *value = 0x6<<8;
        *address = BSP_PIN_MUX_SEL2;
        break;
    case BSP_GPIO_PIN_C3:
        mask = 0xf<<4;
        *value = 0x6<<4;
        *address = BSP_PIN_MUX_SEL2;
        break;
    case BSP_GPIO_PIN_C4:
        mask = 0xf<<16;
        *value = 0x2<<16;
        *address = BSP_PIN_MUX_SEL16;
        break;
    case BSP_GPIO_PIN_C5:
        mask = 0xf<<12;
        *value = 0x6<<12;
        *address = BSP_PIN_MUX_SEL16;
        break;
    case BSP_GPIO_PIN_C6:
        mask = 0xf<<8;
        *value = 0x8<<8;
        *address = BSP_PIN_MUX_SEL16;
        break;
    case BSP_GPIO_PIN_C7:
        mask = 0xf<<4;
        *value = 0x5<<4;
        *address = BSP_PIN_MUX_SEL16;
        break;

    case BSP_GPIO_PIN_D0:
        mask = 0xf<<0;
        *value = 0x5<<0;
        *address = BSP_PIN_MUX_SEL16;
        break;
    case BSP_GPIO_PIN_D1:
        mask = 0xf<<28;
        *value = 0x5<<28;
        *address = BSP_PIN_MUX_SEL15;
        break;
    case BSP_GPIO_PIN_D2:
        mask = 0xf<<24;
        *value = 0x5<<24;
        *address = BSP_PIN_MUX_SEL15;
        break;
    case BSP_GPIO_PIN_D3:
        mask = 0xf<<20;
        *value = 0x5<<20;
        *address = BSP_PIN_MUX_SEL15;
        break;
    case BSP_GPIO_PIN_D4:
        mask = 0xf<<16;
        *value = 0x5<<16;
        *address = BSP_PIN_MUX_SEL15;
        break;
    case BSP_GPIO_PIN_D5:
        mask = 0xf<<12;
        *value = 0x7<<12;
        *address = BSP_PIN_MUX_SEL15;
        break;
    case BSP_GPIO_PIN_D6:
        mask = 0xf<<8;
        *value = 0x6<<8;
        *address = BSP_PIN_MUX_SEL15;
        break;
    case BSP_GPIO_PIN_D7:
        mask = 0xf<<4;
        *value = 0x7<<4;
        *address = BSP_PIN_MUX_SEL15;
        break;
    
    case BSP_GPIO_PIN_E0:
        mask = 0xf<<0;
        *value = 0x7<<0;
        *address = BSP_PIN_MUX_SEL15;
        break;
    case BSP_GPIO_PIN_E1:
        mask = 0xf<<28;
        *value = 0x1<<28;
        *address = BSP_PIN_MUX_SEL17;
        break;
    case BSP_GPIO_PIN_E2:
        mask = 0xf<<24;
        *value = 0x1<<24;
        *address = BSP_PIN_MUX_SEL17;
        break;
    case BSP_GPIO_PIN_E3:
        mask = 0xf<<20;
        *value = 0x1<<20;
        *address = BSP_PIN_MUX_SEL17;
        break;
    case BSP_GPIO_PIN_E4:
        mask = 0xf<<16;
        *value = 0x1<<16;
        *address = BSP_PIN_MUX_SEL17;
        break;
    case BSP_GPIO_PIN_E5:
        mask = 0xf<<12;
        *value = 0x1<<12;
        *address = BSP_PIN_MUX_SEL17;
        break;
    case BSP_GPIO_PIN_E6:
        mask = 0xf<<8;
        *value = 0x1<<8;
        *address = BSP_PIN_MUX_SEL17;
        break;
    case BSP_GPIO_PIN_E7:
        mask = 0xf<<4;
        *value = 0x1<<4;
        *address = BSP_PIN_MUX_SEL17;
        break;

    case BSP_GPIO_PIN_F0:
        mask = 0xf<<0;
        *value = 0x1<<0;
        *address = BSP_PIN_MUX_SEL17;
        break;
    case BSP_GPIO_PIN_F1:
        mask = 0xf<<28;
        *value = 0x2<<28;
        *address = BSP_PIN_MUX_SEL18;
        break;
    case BSP_GPIO_PIN_F2:
        mask = 0xf<<24;
        *value = 0x4<<24;
        *address = BSP_PIN_MUX_SEL18;
        break;
    case BSP_GPIO_PIN_F3:
        mask = 0xf<<20;
        *value = 0x4<<20;
        *address = BSP_PIN_MUX_SEL18;
        break;
    case BSP_GPIO_PIN_F4:
        mask = 0xf<<16;
        *value = 0x6<<16;
        *address = BSP_PIN_MUX_SEL18;
        break;
    case BSP_GPIO_PIN_F5:
        mask = 0xf<<12;
        *value = 0x6<<12;
        *address = BSP_PIN_MUX_SEL18;
        break;
    case BSP_GPIO_PIN_F6:
        mask = 0xf<<24;
        *value = 0x6<<24;
        *address = BSP_PIN_MUX_SEL8;
        break;
    case BSP_GPIO_PIN_F7:
        mask = 0xf<<28;
        *value = 0x6<<28;
        *address = BSP_PIN_MUX_SEL8;
        break;

    case BSP_GPIO_PIN_G0:
        mask = 0xf<<20;
        *value = 0x6<<20;
        *address = BSP_PIN_MUX_SEL8;
        break;
    case BSP_GPIO_PIN_G1:
        mask = 0xf<<16;
        *value = 0x7<<16;
        *address = BSP_PIN_MUX_SEL8;
        break;
    case BSP_GPIO_PIN_G2:
        mask = 0xf<<12;
        *value = 0x7<<12;
        *address = BSP_PIN_MUX_SEL8;
        break;
    case BSP_GPIO_PIN_G3:
        mask = 0xf<<28;
        *value = 0x2<<28;
        *address = BSP_PIN_MUX_SEL9;
        break;
    case BSP_GPIO_PIN_G4:
        mask = 0xf<<24;
        *value = 0x1<<24;
        *address = BSP_PIN_MUX_SEL9;
        break;
    case BSP_GPIO_PIN_G5:
        mask = 0xf<<20;
        *value = 0x0<<20;
        *address = BSP_PIN_MUX_SEL9;
        break;
    case BSP_GPIO_PIN_G6:
        mask = 0xf<<28;
        *value = 0x3<<28;
        *address = BSP_PIN_MUX_SEL13;
        break;
    case BSP_GPIO_PIN_G7:
        mask = 0xf<<24;
        *value = 0x3<<24;
        *address = BSP_PIN_MUX_SEL13;
        break;

    case BSP_GPIO_PIN_H0:
        mask = 0xf<<20;
        *value = 0x3<<20;
        *address = BSP_PIN_MUX_SEL13;
        break;
    case BSP_GPIO_PIN_H1:
        mask = 0xf<<16;
        *value = 0x2<<16;
        *address = BSP_PIN_MUX_SEL13;
        break;
    case BSP_GPIO_PIN_H2:
        mask = 0xf<<28;
        *value = 0x2<<28;
        *address = BSP_PIN_MUX_SEL14;
        break;
    case BSP_GPIO_PIN_H3:
        mask = 0xf<<24;
        *value = 0x1<<24;
        *address = BSP_PIN_MUX_SEL12;
        break;
    case BSP_GPIO_PIN_H4:
        mask = 0xf<<28;
        *value = 0x1<<28;
        *address = BSP_PIN_MUX_SEL12;
        break;
    case BSP_GPIO_PIN_H5:
        mask = 0xf<<0;
        *value = 0x1<<0;
        *address = BSP_PIN_MUX_SEL8;
        break;
    case BSP_GPIO_PIN_H6:
        break;

    case BSP_GPIO_PIN_H7:
        break;

    case BSP_UART1_PIN:
        break;
    case BSP_UART2_PIN:
        break;
    default:
        break;
    }

    return mask;
}

/*****************************************************************************
 �� �� ��  : get_gpio_value
 ��������  : �õ�gpio ���ŵĸߵ͵�ƽֵ
 �������  : gpio_id
 �������  : ��
 �� �� ֵ  : 0��1
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��15��
    ��    ��   : ��־
    �޸�����   : �����ɺ���
    �޸��ˣ� ��־

*****************************************************************************/
static int get_gpio_value(unsigned int gpio_id)
{
    unsigned int value;

    value = (RTL_R32(BSP_GPIO_DAT_REG(gpio_id)));

    if (value & (1 << BSP_GPIO_BIT(gpio_id))) {
        return 0;
    } else {
        return 1;
    }
}

/*****************************************************************************
 �� �� ��  : set_gpio_value
 ��������  : ����gpio ���ŵĸߵ͵�ƽֵ
 �������  : gpio_id�� value:0Ϊ�ͣ�1Ϊ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��15��
    ��    ��   : ��־
    �޸�����   : �����ɺ���
    �޸��ˣ� ��־ 
*****************************************************************************/
static void set_gpio_value(unsigned int gpio_id, unsigned int value)
{
    unsigned int reg_val;

    reg_val = REG32(BSP_GPIO_DAT_REG(gpio_id));
    if (value) {
        reg_val &= ~(1 << BSP_GPIO_BIT(gpio_id));
    } else {
        reg_val |= (1 << BSP_GPIO_BIT(gpio_id));
    }
    REG32(BSP_GPIO_DAT_REG(gpio_id)) = reg_val;

    return;
}

/*****************************************************************************
 �� �� ��  : set_gpio_mux_mode
 ��������  : ����gpio ���ŵĸ���ģʽ
 �������  : gpio_id�� mode:0 ��������ģʽ��1ΪGPIOģʽ
 �������  : ��
 �� �� ֵ  : ��
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��15��
    ��    ��   : ��־
    �޸�����   : �����ɺ���
    �޸��ˣ� ��־ 
*****************************************************************************/
static void set_gpio_mux_mode(unsigned int gpio_id, unsigned int mode)
{
    unsigned int mask, value, address;

    mask = rtl819x_gpio_mux(gpio_id, &value, &address);
    if (mode) {
        RTL_W32(address, (RTL_R32(address) & ~(mask) | (value)));
    } else {
        RTL_W32(address, (RTL_R32(address | mask)));
    }

    return;
}

/*****************************************************************************
 �� �� ��  : set_gpio_direction
 ��������  : ����gpio ���ŵ������������
 �������  : gpio_id�� direction:0 �����1ΪG
 �������  : ��
 �� �� ֵ  : ��
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��15��
    ��    ��   : ��־
    �޸�����   : �����ɺ���
    �޸��ˣ� ��־ 
*****************************************************************************/
static void set_gpio_direction(unsigned int gpio_id, unsigned int direction)
{
    unsigned int value;

    set_gpio_mux_mode(gpio_id, 1);

    value = RTL_R32(BSP_GPIO_CNR_REG(gpio_id));
    value &=(~(1 << BSP_GPIO_BIT(gpio_id)));
     
    RTL_W32(BSP_GPIO_CNR_REG(gpio_id), value);

    /* set gpio as input or output */
    value = RTL_R32(BSP_GPIO_DIR_REG(gpio_id));
    if (!direction){
        value |= (1 << BSP_GPIO_BIT(gpio_id));
    } else {
        value &= (~(1 << BSP_GPIO_BIT(gpio_id)));  
    }

    RTL_W32(BSP_GPIO_DIR_REG(gpio_id), value);

    return;
}


#ifdef __CONFIG_SUPPORT_SWITCH_LED_BY_GPIO__
unsigned int g_set_lan_wan_led = 0;
#endif

/*****************************************************************************
 �� �� ��  : set_lan_port_as_gpio
 ��������  : ����lan_portΪGPIO
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��15��
    ��    ��   : cz
    �޸�����   : �޸�lan�ڵ�GPIOP����,�ӿ��Ż�����2.0�汾

*****************************************************************************/
void set_lan_port_as_gpio(void)
{
#ifdef __CONFIG_SUPPORT_SWITCH_LED_BY_GPIO__
    g_set_lan_wan_led = 1; /*��ʹ�ö�ʱ���Զ�����*/
#else
#if defined(CONFIG_RTL_8367R_SUPPORT)
    /*���ʹ��һ���led���ڴ�add ����8367 led ����*/
#else
    set_gpio_direction(54, 0);
    set_gpio_direction(55, 0);
    set_gpio_direction(56, 0);
    set_gpio_direction(57, 0);
    set_gpio_direction(58, 0);
#endif
#endif
    return;
}

/*****************************************************************************
 �� �� ��  : set_lan_port_as_led
 ��������  : ����lan_portΪledģʽ�������ڵ�ģʽ
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��15��
    ��    ��   : liquan
    �޸�����   : �����ɺ���
    �޸��ˣ� ��־ ����v2.0�ӿ�

*****************************************************************************/
void set_lan_port_as_led(void)
{
#ifdef __CONFIG_SUPPORT_SWITCH_LED_BY_GPIO__
        g_set_lan_wan_led = 0; /*ʹ�ö�ʱ���Զ����ƻָ�����*/
#else
#if defined(CONFIG_RTL_8367R_SUPPORT)
    /*�ڴ����8367����led�ƺ������ֵ*/
#else
    set_gpio_mux_mode(54, 1);
    set_gpio_mux_mode(55, 1);
    set_gpio_mux_mode(56, 1);
    set_gpio_mux_mode(57, 1);
    set_gpio_mux_mode(58, 1);
#endif
#endif
    return;
}

/*****************************************************************************
 �� �� ��  : lan_wan_port_led_on_off
 ��������  : ����lan_wan ͬʱ������ͬʱ����
 �������  : val: 0 �� 1��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : �޸�lan_wan�ڵ�GPIOP����
    �޸����ڣ�2018/6/15
    �޸��ˣ� ��־ ����v2.0�ӿ�

*****************************************************************************/
void lan_wan_port_led_on_off(int val)
{
    set_lan_port_as_gpio();

#if defined(__CONFIG_SUPPORT_SWITCH_LED_BY_GPIO__)
    set_lan_led_on_off(val);
#if defined(__CONFIG_WAN_LED_GPIO_NUM__)
    cyg_thread_delay(2);
    set_wan_led_on_off(val);
#endif

#else
#if defined(CONFIG_RTL_8367R_SUPPORT)
    /*���ʹ��8367�Ӷ���������ڵƣ��˴���ӿ��ƺ���*/
#else
    set_gpio_value(54, val);
    set_gpio_value(55, val);
    set_gpio_value(56, val);
    set_gpio_value(57, val);
    set_gpio_value(58, val);
#endif
#endif
}

/*****************************************************************************
 �� �� ��  : lan_port_led_on_off
 ��������  : ��������lan�ڵĵ�
 �������  : int val  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : �޸�lan�ڵ�GPIOP����
    �޸����ڣ�2018/6/15
    �޸��ˣ� ��־ ����v2.0�ӿ�

*****************************************************************************/
void lan_port_led_on_off(int val)
{
    lan_wan_port_led_on_off(val);

    return;
}

/*****************************************************************************
 �� �� ��  : tenda_button_status
 ��������  : ��ȡ����ֵ
 �������  : int gpio  
 �������  : ��
 �� �� ֵ  : 0 �� 1 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : ʹ��ͳһ��ȡgpioֵ����
    �޸����ڣ�2018/6/15
    �޸��ˣ� ��־ ����v2.0�ӿ�

*****************************************************************************/
int tenda_button_status(unsigned int gpio)
{
    int status; 

    status = get_gpio_value(gpio);
    if (status == 1) {
        return BUTTON_PUSH_UP;
    } else {
        return BUTTON_PUSH_DOWN;
    }
}

/*****************************************************************************
 �� �� ��  : set_lan_led_on_off
 ��������  : ��ͨ gpio ��Ϊ lan �ڵ�ʹ��ʱ
 �������  : int value  
 �������  : ��
 �� �� ֵ  : ��
 
 �޸���ʷ      :
    ��    ��   : cz
    ��������   : ����lan�ڵƵ�״ֵ̬
    �޸����ڣ�2018/6/15

*****************************************************************************/
#if defined(__CONFIG_SUPPORT_SWITCH_LED_BY_GPIO__)
void set_lan_led_on_off(unsigned int value)
{
#if defined(__CONFIG_SW_LED_LOW_ACTIVE__)
    set_gpio_value(__CONFIG_LAN_LED_GPIO_NUM__, ((value == LED_ON) ? LED_OFF : LED_ON));
#else
    set_gpio_value(__CONFIG_LAN_LED_GPIO_NUM__, value);
#endif
}

/*****************************************************************************
 �� �� ��  : set_wan_led_on_off
 ��������  : ��ͨ gpio ��Ϊ wan�ڵ�ʹ��ʱ
 �������  : int value  
 �������  : ��
 �� �� ֵ  : ��
 
 �޸���ʷ      :
    ��    ��   : cz
    ��������   : ����wan�ڵƵ�״ֵ̬
    �޸����ڣ�2018/6/15

*****************************************************************************/
#if defined(__CONFIG_WAN_LED_GPIO_NUM__)
void set_wan_led_on_off(unsigned int value)
{
#if defined(__CONFIG_SW_LED_LOW_ACTIVE__)
    set_gpio_value(__CONFIG_WAN_LED_GPIO_NUM__, ((value == LED_ON) ? LED_OFF : LED_ON));
#else
    set_gpio_value(__CONFIG_WAN_LED_GPIO_NUM__, value);
#endif  
    return;
}
#endif
#endif /*__CONFIG_SUPPORT_SWITCH_LED_BY_GPIO__ end */

/*****************************************************************************
 �� �� ��  : sys_led_turn_on_off
 ��������  : ʹ��ͳһ����gpioֵ��������ϵͳ��״̬
 �������  : int val  
 �������  : ��
 �� �� ֵ  : ��
 
 �޸���ʷ      :
    �޸����ڣ�2018/6/15
    �޸��ˣ�cz

*****************************************************************************/
void sys_led_turn_on_off(int val)
{
    if(val) {
        set_gpio_value(SYSTEM_LED_PIN,LED_ON);
    } else {
        set_gpio_value(SYSTEM_LED_PIN,LED_OFF);
    }

    return;
}

#ifdef __CONFIG_WPS_RTK__
static unsigned int wps_led_blink_flag = 0;
static unsigned int wps_led_toggle_flag = 0;

/*****************************************************************************
 �� �� ��  : wps_led_on_off
 ��������  : ����wpsָʾ�ƿ�����
 �������  : int val  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��9��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
void wps_led_on_off(int val)
{
    /* �����WPS����˸��ʱ��ϵͳ�Ʋ����� */
    if(wps_led_blink_flag) {
        if (val == LED_OFF) {
             set_gpio_value(SYSTEM_LED_PIN, LED_OFF);
        } else {
             set_gpio_value(SYSTEM_LED_PIN, LED_ON);
        }
    }
    cyg_thread_delay(2);
    set_gpio_value(WPS_LED_PIN, val);
}

void wps_led_off(void)
{
    wps_led_on_off(LED_OFF);
    wps_led_blink_flag = 0;
    printf("%s,wps_led_blink_flag= %u\n",__func__,wps_led_blink_flag);
}

void wps_led_on(void)
{
    wps_led_on_off(LED_ON);
    wps_led_blink_flag = 0;
    printf("%s,wps_led_blink_flag= %u\n",__func__,wps_led_blink_flag);
}

void wps_led_blink(void)
{
    wps_led_on_off(LED_OFF);
    wps_led_blink_flag = 1;
    wps_led_toggle_flag = 1;
    printf("%s,wps_led_blink_flag= %u\n",__func__,wps_led_blink_flag);
}

/*****************************************************************************
 �� �� ��  : wps_led_timer
 ��������  : wpsָʾ�ƶ�ʱ���ӿ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��9��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
void wps_led_timer()
{
    if (wps_led_blink_flag==1)
    {
        if (wps_led_toggle_flag) 
        {
            wps_led_on_off(LED_OFF);
        }
        else 
        {
            wps_led_on_off(LED_ON);
        }
        wps_led_toggle_flag = wps_led_toggle_flag ? 0 : 1;
    }
}


/*****************************************************************************
 �� �� ��  : wps_led_control
 ��������  : �ṩ��wpsģ��Ľӿڣ����ڿ���wpsָʾ��
 �������  : int value  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��9��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
void wps_led_control(int value)
{
    if (value == 0)
    {
        wps_led_off();
    }   
    else if (value == 1)
    {       
        wps_led_on();
    }   
    else if (value == 2)
    {       
        wps_led_blink();
    }   
}
#endif


#ifdef __CONFIG_LED__
extern int set_sys_led_turn(int turn_flag);
int tenda_set_all_led_off(void)
{
    lan_wan_port_led_on_off(0);
#ifdef __CONFIG_WPS_RTK__
    wps_led_control(0); 
#endif
    set_sys_led_turn(0);
    sys_led_turn_on_off(0); 
}
/*****************************************************************************
 �� �� ��  : tenda_set_all_led_on
 ��������  : �����������е�LED����, �ָ�����ģʽ
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��10��
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
int tenda_set_all_led_on(void)
{
    set_lan_port_as_led();

    set_sys_led_turn(1);
    sys_led_turn_on_off(1);
    return 0;
}
#endif

/*****************************************************************************
 �� �� ��  : tenda_all_led_on_off
 ��������  : ��������LED��
 �������  : int val  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : ledȫ��ȫ�ؽӿ�����
*****************************************************************************/
void tenda_all_led_on_off(int val)
{
    char buf[32];
    /***set lan_wan_led***/
    lan_wan_port_led_on_off(val);
#ifdef __CONFIG_WPS_RTK__
    wps_led_control(val); 
#endif
    sys_led_turn_on_off(val); 
   /*********2.4_5g  led**********/
    if (val == 0) {
        sprintf(buf,"wlan0 led 0");
        run_clicmd(buf);
        memset(buf,0x0,sizeof(buf));
        sprintf(buf,"wlan1 led 0");
        run_clicmd(buf);
    } else {
        sprintf(buf,"wlan0 led 1");
        run_clicmd(buf);
        memset(buf,0x0,sizeof(buf));
        sprintf(buf,"wlan1 led 1");
        run_clicmd(buf);
    }
}
