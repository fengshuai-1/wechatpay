#include "Flash.h"

void Flash_write(uint32_t addr, uint32_t *data)
{
    uint32_t err0r = 0;
    FLASH_EraseInitTypeDef flash_earse_init;
    flash_earse_init.TypeErase = FLASH_TYPEERASE_PAGES;
    flash_earse_init.PageAddress = addr;
    flash_earse_init.NbPages = 1;
    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase( &flash_earse_init, &err0r);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addr,*data);
    HAL_FLASH_Lock();
}

void Flash_read( uint32_t addr, uint32_t *data )
{
    *data = *(__IO uint32_t*)(addr);
}

