/*
 *  simple_char driver  - Simplest Character Driver ($Revision: 1 $)
 *
 *  Copyright (C) 2015 Aswin Achuth Shankar  <devachuthshankar@gmail.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <asm/uaccess.h>
dev_t dev;
int debug=1;
int max=10;
#define BUFFER_SIZE 18000
struct cdev *mycdev;
static int ptr=0;
char *p,*q;
static char device_buffer[BUFFER_SIZE];
/////////////////////////////////////////////////////////////////////////
int dev_open(struct inode *inode,struct file *filp)
{
printk("\nDevice opened\n");
if ( (filp->f_flags & O_ACCMODE) == O_WRONLY)
{
p=(char *)device_buffer;
ptr=0;
}
else if((filp->f_flags&O_ACCMODE)==O_RDONLY)
q=(char *)device_buffer;

return 0;
}
///////////////////////////////////////////////////////////////////////

int dev_close(struct inode *inode,struct file *filp)
{
printk("\nDevice closed\n");
return 0;
}

///////////////////////////////////////////////////////////////////////

ssize_t dev_read(struct file *fp,char *buff,ssize_t length,loff_t *pos)
{
length=ptr;
printk("\nread starts\n");
printk("\nlength: %ld  *pos= %ld\n",length,*pos);
copy_to_user(buff,q,max);
q+=max;
*pos+=max;
printk("\nread ends\n");
if(ptr-=max)
return max;
else 
return 0;
}

///////////////////////////////////////////////////////////////////////

ssize_t dev_write(struct file *filp,const char *buff,size_t length,loff_t *pos)
{
printk("\nwrite starts\n");
printk("\nlength: %ld  *pos= %ld\n",length,*pos);
copy_from_user(p, buff, max);
p+=max;
*pos+=max;
printk("\nwrite ends\n");
ptr+=max;
return max;
}

///////////////////////////////////////////////////////////////////////

struct file_operations fops={
.owner=THIS_MODULE,
.open=dev_open,
.read=dev_read,
.write=dev_write,
.release=dev_close,
};


///////////////////////////////////////////////////////////////////////

int dev_init(void)
{
int res;
if(debug)
	printk(KERN_INFO"\ndev_init\n");
res=alloc_chrdev_region(&dev,0,1,"simple_char");
if(res<0)
	printk(KERN_ALERT"\nDevice no Allocation failed\n");
else
	if(debug)
		printk(KERN_INFO"\nDevice no Allocation Successfull %d\n",MAJOR(dev));
mycdev=cdev_alloc();
mycdev->ops=&fops;
mycdev->owner=THIS_MODULE;
res=cdev_add(mycdev,dev,1);
if(res<0)
        printk(KERN_ALERT"\nAdding character device into the kernel failed\n");
else
        if(debug)
        	printk(KERN_INFO"\nAdding character device into the kernel successfull\n");

return 0;
}

///////////////////////////////////////////////////////////////////////

void dev_exit(void)
{
if(debug)
	printk(KERN_INFO"\ndev_exit\n");
cdev_del(mycdev);
unregister_chrdev_region(dev,1);
if(debug)
	printk(KERN_DEBUG"\nfreed Device no\n");
}

///////////////////////////////////////////////////////////////////////
module_init(dev_init);
module_exit(dev_exit);
///////////////////////////////////////////////////////////////////////
