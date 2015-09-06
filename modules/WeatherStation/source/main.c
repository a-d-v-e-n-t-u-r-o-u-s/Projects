#define PC_ODR *(unsigned char*)0x500A
#define PC_IDR *(unsigned char*)0x500B
#define PC_DDR *(unsigned char*)0x500C
#define PC_CR1 *(unsigned char*)0x500D
#define PC_CR2 *(unsigned char*)0x500E

#define PE_ODR *(unsigned char*)0x5014
#define PE_IDR *(unsigned char*)0x5015
#define PE_DDR *(unsigned char*)0x5016
#define PE_CR1 *(unsigned char*)0x5017
#define PE_CR2 *(unsigned char*)0x5018

int main(void)
{
    long d;

    PE_DDR = 0x80;
    PE_CR1 = 0x80;
    PC_DDR = 0x80;
    PC_CR1 = 0x80;
    PC_ODR ^= 0x80;

    do
    {
        PE_ODR ^= 0x80;
        PC_ODR ^= 0x80;

        for(d = 0; d < 10000; d++)
        {
        }

    } while(1);
}
