#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
    case ET_NONE:
      type_str = "ET_NONE (No file type)";
      break;
    case ET_REL:
      type_str = "ET_REL (Relocatable file)";
      break;
    case ET_EXEC:
      type_str = "ET_EXEC (Executable file)";
      break;
    case ET_DYN:
      type_str = "ET_DYN (Shared object file)";
      break;
    case ET_CORE:
      type_str = "ET_CORE (Core file)";
      break;
    default:
      type_str = "Unknown";
      break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  // 模拟不同的ELF类型
  uint16_t elf_types[] = {ET_REL, ET_DYN};
  const char *type_names[] = {"Relocatable", "Shared Object/PIE"};
  
  for (int i = 0; i < 2; i++) {
    printf("Checking ELF type: %s\n", type_names[i]);
    print_elf_type(elf_types[i]);
    printf("\n");
  }
  
  return 0;
}
