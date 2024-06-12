#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "util.h"
//#include "calculator.h"
//#include "mem.h"


void start_kernel() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();
    print_string("*******************************************************\n");
    print_string("*******************************************************\n");
    print_string("*******************************************************\n");
    print_string("*******************************************************\n");
    print_string("*****************Welcome to RIOS***********************\n");
    print_string("*******************************************************\n");
    print_string("*******************************************************\n");
    print_string("*******************************************************\n");
    print_string("*******************************************************\n");
    
    print_string("> ");
}

float string_to_float(const char *str) {
    float result = 0.0f;
    int sign = 1;
    int decimal_found = 0;
    float decimal_place = 0.1f; 

    if (*str == '-') {
        sign = -1;
        str++;
    }

    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            if (!decimal_found) {
                result = result * 10 + (*str - '0');
            } else {
                result = result + (*str - '0') * decimal_place;
                decimal_place *= 0.1f;
            }
        } else if (*str == '.') { 
            decimal_found = 1;
        } else { 
            return 0.0f;
        }
        str++;
    }

    return result * sign;
}

float compare(char s[],char s2[]) {
    int l = 0;
    char a[10],b[10];
    int i = 0,j=0;
    int c=0,k=0,k1=0;
    while (s[l] != '\0') {

        if (s[l] == '[')
            c=1;
        if (s[l] == ',')
        {
            k=1;
            c=0;
        }

        if(c==1 && s[l+1]!=',')
        {
        a[i++]=s[l+1];
        }
        if(k==1 && s[l+1]!=']')
        {
            b[j++]=s[l+1];
        }


        l++;
        //printf("%d ",c);
    }
    a[i] = '\0'; // Null-terminate the string
    
    if(compare_string(s2, "sum")==0)
    {
        float ans=string_to_float(a)+string_to_float(b);
    	return ans;
    }
    if(compare_string(s2, "sub")==0)
    {
        float ans=string_to_float(a)-string_to_float(b);
    	return ans;
    }
    if(compare_string(s2, "div")==0)
    {
        float ans=string_to_float(a)/string_to_float(b);
    	return ans;
    }
    if(compare_string(s2, "mul")==0)
    {
        float ans=string_to_float(a)*string_to_float(b);
    	return ans;
    }


}

void execute_command(char *input) {
    if (compare_string(input, "EXIT") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if(compare_string(input, "SUM")==0)
    {
    	

    	float ans=compare(input,"sum");
    	print_float(ans); 
        print_string("\n> ");
    }
    else if(compare_string(input, "SUB") == 0)
    {

	float ans=compare(input,"sub");
	print_float(ans); 
	print_string("\n> ");

    }
     else if(compare_string(input, "DIV") == 0)
    {

	float ans=compare(input,"div");
	print_float(ans); 
	print_string("\n> ");

    }
     else if(compare_string(input, "MUL") == 0)
    {

	float ans=compare(input,"mul");
	print_float(ans); 
	print_string("\n> ");

    }
    else if(compare_string(input, "CLR") == 0)
    {
    clear_screen();
    print_string("\n> ");
    }
    
    else if (compare_string(input, "") == 0) {
        print_string("\n> ");
    }
    else {
        print_string("Unknown command: ");
        print_string(input);
        print_string("\n> ");
    }
}
