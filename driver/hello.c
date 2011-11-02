#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("Dual BSD/GPL") /* Evita reclamacoes de kernel */

staticint hellok(void) {
printk(KERN_ALERT "Kello, Hernel! :]\n");

/* Coisas que fazem os drivers de dispositivos. */

return 0;
}


staticvoid byek(void) {
printk(KERN_ALERT "Bye, Kernel! :[\n");
}


module_init(hellok);
module_exit(byek);
