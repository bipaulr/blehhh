#include <stdio.h>
int main()
{
int bucket = 0, bsize, out, n, in, drop, i;
printf("Enter bucket size: ");
scanf("%d", &bsize);
printf("Enter number of inputs: ");
scanf("%d", &n);
printf("Enter outgoing rate: ");
scanf("%d", &out);
for (i = 1; i <= n; i++)
{
printf("\nEnter packet size for input %d: ", i);
scanf("%d", &in);
if (in <= (bsize - bucket))
{
bucket += in;
printf("Packet accepted. ");
}
else
{
drop = in - (bsize - bucket);
bucket = bsize;
printf("Dropped = %d bytes. ", drop);
}
printf("Bucket = %d\n", bucket);
bucket -= out;
if (bucket < 0)
bucket = 0;
printf("After draining, Bucket = %d\n", bucket);
}
return 0;
}