#include <linux/init.h>

#include <linux/module.h>

#include <linux/types.h>

#include <linux/kdev_t.h>

#include <linux/fs.h>


MODULE_LICENSE("Dual BSD/GPL"); /* Evita reclamacoes de kernel */

/* Variáveis globais a esse arquivo. Podemos precisar delas mais tarde */

static int major;

static dev_t dev;

static int hellok(void) {

	 printk(KERN_ALERT "Kello, Hernel! :]\n");

	  /* Reigstra um driver com 3 minor numbers com o nome "driver" */

	  if (!alloc_chrdev_region(&dev, 0, 3, "driver")) {

		   printk(KERN_ALERT "Houve um problema alocando os números de driver\n");

		    //exit(1);

		     }

	   major = MAJOR(dev);

	    printk(KERN_ALERT "Nosso major number é %d\n", major);

	     /* Outras coisas que fazem os drivers de dispositivos */

	     return 0;

}

static void byek(void) {

	 printk(KERN_ALERT "Bye, Kernel! :[\n");

	  /* Desregistra o nosso driver */

	  unregister_chrdev_region((dev_t) &dev, 3);

}

module_init(hellok);

module_exit(byek);
