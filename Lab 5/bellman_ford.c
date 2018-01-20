// Nodes should be named 0, 1, 2,...
// The input file should contain space separated value: The first two values being the number of nodes
// and the number of connections. These are followed by the nodes and weight of each connection/edge.
// Each bi-direcional edge should be given as two uni-directional edges.
#include <stdio.h>
#include <stdlib.h>
struct edge{
    int x, y;
    int cost;
};
int n;
int getInput(FILE*);
int getNextNode(int*, int*);
int main(int argc, char* argv[])
{
    int i, j, k, node, connections;
    int *distFromSource, *parent;
    struct edge* edges;
    FILE* fp;

    if(argc != 2)
    {
        printf("Usage: ./a.out <input file>");
        exit(0);
    }

    // Taking input
    fp = fopen(argv[1], "r");
    n = getInput(fp);
    connections = getInput(fp);
    distFromSource = (int*)malloc(n*sizeof(int));
    parent = (int*)malloc(n*sizeof(int));
    edges = (struct edge*)malloc(connections*sizeof(struct edge));
    for(i = 0; i < connections; i++)
    {
        edges[i].x = getInput(fp);
        edges[i].y = getInput(fp);
        edges[i].cost = getInput(fp);
        if(edges[i].x == 0)
            distFromSource[edges[i].x] = edges[i].cost;
        else if(edges[i].y == 0)
            distFromSource[edges[i].y] = edges[i].cost;
    }
    fclose(fp);

    printf("\nSource\t\tDestination\t\tCost\n");
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
            distFromSource[j] = 100000;
        distFromSource[i] = 0;
        parent[i] = -1;
        for(j = 0; j < n-1; j++)
            for(k = 0; k < connections; k++)
                if(distFromSource[edges[k].y] > distFromSource[edges[k].x] + edges[k].cost)
                {
                    distFromSource[edges[k].y] = distFromSource[edges[k].x] + edges[k].cost;
                    parent[edges[k].y] = edges[k].x;
                }
        for(j = 0; j < 50; j++)
            printf("-");
        printf("\n");
        for(j = 0; j < n; j++)
            printf("%d\t\t%d\t\t\t%d\n", i, j, distFromSource[j]);
    }
    printf("\n");
    return 0;
}

int getInput(FILE* fp)
{
    char str[10];
    if(fscanf(fp, "%s ", str) != 1)
    {
        printf("Nothing left to read...\n");
        exit(1);
    }
    return atoi(str);
}