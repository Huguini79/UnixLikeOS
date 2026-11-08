#include <include/unix/printk.h>
#include <include/unix/console.h>

#include <stdint.h>
#include <stddef.h>

extern void load_page_directory();
extern void enable_paging();

/*
	- EL KERNEL TIENE ACCESO A TODA LA MEMORIA, POR LO TANTO, SIMPLEMENTE, MAPEAMOS TODO PARA QUE SEA IGUAL LA DIRECCIÓN LÓGICA LA FÍSICA
	- EL USUARIO, TIENE TODA SU MEMORIA A PARTIR DEL 1 GB de MEMORIA (0x40000000-0xFFFFFFFF), por lo tanto, tiene 3GB de memoria los procesos de usuario
*/

uint32_t kernel_page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_table[1024][1024] __attribute__((aligned(4096)));

void set_kernel_paging()
{
	long flags = 3;
	uint8_t* kernel_start_address = (uint8_t*)0x0000;
	uint32_t offset = 0; // ESTO LO USAMOS PARA AVANZAR DE PÁGINA EN PÁGINA (OFFSET += 4096 bytes -> 4KB (cada página son 4KB)) 

	printk("INIT PAGING ");
	for (int i = 0; i < 1024; i++)
	{
		for (int j = 0; j < 1024; j++)
		{
			page_table[i][j] = ((uint32_t)kernel_start_address + offset) | flags;
			offset += 4096;
		}

		kernel_page_directory[i] = ((uint32_t)page_table[i]) | flags;
	}

	load_page_directory(kernel_page_directory);
	success();
	printk("ENABLE PAGING (PG bit CR0) ");
	enable_paging();
	success();
}

long return_page_directory()
{
	return (long)kernel_page_directory;
}

/*
void set_user_paging()
{

	int offset = 0;
	*(user_address) = 0; // Rellenar con ceros esa dirección de memoria
	
}
*/
