#include <stdio.h>
#define INF 999
struct router {
int dist[10];
int from[10];
} rt[10];
int main()
{
int costmat[10][10];
int n, i, j, k, count;
printf("Enter number of routers: ");
scanf("%d", &n);
printf("Enter cost matrix (use %d for no direct link):\n", INF);
for (i = 0; i < n; i++)
for (j = 0; j < n; j++)
{
scanf("%d", &costmat[i][j]);
rt[i].dist[j] = costmat[i][j];
rt[i].from[j] = j;
}
do {
count = 0;
for (i = 0; i < n; i++)
for (j = 0; j < n; j++)
for (k = 0; k < n; k++)
if (rt[i].dist[k] != INF && rt[k].dist[j] != INF &&
rt[i].dist[k] + rt[k].dist[j] < rt[i].dist[j])
{
rt[i].dist[j] = rt[i].dist[k] + rt[k].dist[j];
rt[i].from[j] = k;
count++;
}
} while (count != 0);
for (i = 0; i < n; i++)
{
printf("\nRouting table of Router %d:\n", i);
printf("%-15s %-10s %-10s\n", "Destination", "Distance", "Next Hop");
for (j = 0; j < n; j++)
printf("%-15d %-10d %-10d\n", j, rt[i].dist[j], rt[i].from[j]);
}
return 0;
}