// Nodes should be named 0, 1, 2,...
// The input file should contain space separated value: The first two values being the number of nodes
// and the number of connections. These are followed by the nodes and weight of each connection/edge.
#include <stdio.h>
#include <stdlib.h>
int n, determined;
int getInput(FILE*);
int getNextNode(int*, int*);
int main(int argc, char* argv[])
{
    int i, j, x, y, node, connections, temp, src, dest;
    int **weight, *determinedNodes, *distFromSource, *parent;
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

    weight = (int**)malloc(n*sizeof(int*));
    for(i = 0; i < n; i++)
        weight[i] = (int*)malloc(n*sizeof(int));
    determinedNodes = (int*)malloc(n*sizeof(int));
    distFromSource = (int*)malloc(n*sizeof(int));
    parent = (int*)malloc(n*sizeof(int));
    printf("\nSource: ");
    scanf("%d", &src);
    printf("\nDestination: ");
    scanf("%d", &dest);
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
            weight[i][j] = 100000;
        determinedNodes[i] = 0;
        parent[i] = -1;
    }
    for(i = 0; i < connections; i++)
    {
        x = getInput(fp);
        y = getInput(fp);
        temp = getInput(fp);
        weight[x][y] = temp;
        weight[y][x] = temp;
    }
    fclose(fp);

    // Initialization
    determined = 0;
    determinedNodes[src] = 1;
    determined++;
    for(i = 0; i < n; i++)
    {
        distFromSource[i] = weight[src][i];
        if(src != i)
            parent[i] = src;
    }

    // as long as the number of determined nodes is n
    while(determined < n)
    {
        //find a node that is not in determinedNodes and with minimum distFromSource
        node = getNextNode(determinedNodes, distFromSource);
        determinedNodes[node] = 1;
        determined++;

        for(i = 0; i < n; i++)
            if(i != src)
                if(weight[node][i] != 100000)
                    if(distFromSource[i] > distFromSource[node]+weight[node][i])
                    {
                        parent[i] = node;
                        distFromSource[i] = distFromSource[node]+weight[node][i];
                    }
    }

    // Printing output
    printf("\nPath: ");
    temp = parent[dest];
    printf("%d <- ", dest);
    while(temp != -1)
    {
        printf("%d", temp);
        temp = parent[temp];
        if(temp != -1)
            printf(" <- ");
    }
    printf("\n\nCost: %d\n\n", distFromSource[dest]);
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

int getNextNode(int* determinedNodes, int* distFromSource)
{
    int min = -1, i;
    for(i = 0; i < n; i++)
    {
        if(determinedNodes[i] == 0)
        {
            if(min == -1)
               min = i;
            else
            {
                if(distFromSource[min] >= distFromSource[i])
                    min = i;
            }
        }
    }
    return min;
}