//========================================
//
//  Dijkstra.c | C Implementation of the Dijkstra Algorithm
//  Authors: Abdil Kaya & Robin Janssens
//
//========================================


#include "testd.h"
#define debug

int main()
{
    //Create an array of nodes describing the map
    NodeStruct Nodes[MapSize];
    //Populate the members Neighbours[] and Distance[] in these nodes.
#ifdef debug
    printf("Running ReadNodes\n");
#endif
    ReadNodes(Nodes,MapSize);
    //Calculate path using dijkstra method.
#ifdef debug
    printf("\nRunning Dijkstra\n");
#endif
    Dijkstra(Nodes,MapSize,0,8);

    return 0;
}



int Dijkstra(NodeStruct *Nodes, int MapSize, int Start, int Finish)
{
    Current=Start;


#ifdef debug
    printf("Init dijkstra");
#endif
    InitDijkstra(Nodes, MapSize, Start);

    for (i=0; i <MapSize; ++i)
        toVisit[i]=-1;

    //define distance value of neighbours and queue them without sorting
    n=0;
    for(i=0; i<MapSize; i++)
    {
        for(k=0; k<4; k++)
        {
            if(Nodes[Current].Neighbours[k]!=-1)
                if(Nodes[Nodes[Current].Neighbours[k]].Visited==0 && Nodes[Nodes[Current].Neighbours[k]].Queued==0)
                {

                    if( Nodes[Nodes[Current].Neighbours[k]].DV > ( Nodes[Current].DV + Nodes[Current].Distance[k] ) )
                        Nodes[Nodes[Current].Neighbours[k]].DV = ( Nodes[Current].DV + Nodes[Current].Distance[k] );


                    toVisit[n]=Nodes[Current].Neighbours[k];
                    Nodes[Nodes[Current].Neighbours[k]].Queued=1;

                    n++;
                }
        }
        //The neighbours of this current node are marked for visitation. Now let's sort.
        Nodes[Current].Visited=1;
        Current=SortQueue(Nodes,toVisit);
    }

#ifdef debug
    printf("Sorted Queue\n");
#endif

    for(n=0; n<MapSize; n++)
    {
#ifdef debug
        printf("%d ", toVisit[n]);
#endif
    }

    return 0;
}

void ReadNodes(NodeStruct *Nodes, int MapSize)
{
    int i = 0;
    fp=fopen (Graph, "r");

    if(fp!=NULL)
    {
        while(fscanf(fp, "%d %d %d %d %d %d %d %d", &Nodes[i].Neighbours[0], &Nodes[i].Neighbours[1], &Nodes[i].Neighbours[2], &Nodes[i].Neighbours[3], &Nodes[i].Distance[0], &Nodes[i].Distance[1], &Nodes[i].Distance[2], &Nodes[i].Distance[3]) != EOF && i<MapSize)
        {
#ifdef debug
            printf("%d %d %d %d %d %d %d %d\n", Nodes[i].Neighbours[0], Nodes[i].Neighbours[1], Nodes[i].Neighbours[2], Nodes[i].Neighbours[3], Nodes[i].Distance[0], Nodes[i].Distance[1], Nodes[i].Distance[2], Nodes[i].Distance[3]);
#endif
            i++;
        }
        fclose(fp);
    }
    else
        printf("File does not exist.");
}

void InitDijkstra(NodeStruct *Nodes, int MapSize, int Start)
{
    int j;
    //Initial member values of NodeStruct
    for(j=0; j<MapSize; j++)
    {

        Nodes[j].Visited=0;
        Nodes[j].Queued=0;
        if(j==Start)
            Nodes[j].DV=0;
        else
            Nodes[j].DV=999;
        Nodes[j].Previous=-1;

#ifdef debug
        printf("%d, %d, %d\n",Nodes[j].Visited,Nodes[j].DV,Nodes[j].Previous);
#endif
    }
}

int SortQueue(NodeStruct *Nodes, int *toVisit)
{
    int i,j,flag,Next;
    for(i=0; i<5; i++)
    {

        flag=0;
        for(j=0; j<4-i; j++)
        {
            if( toVisit[j]!=-1 && toVisit[j+1]!=-1)
            {
                if(Nodes[toVisit[j]].DV>Nodes[toVisit[j+1]].DV || (Nodes[toVisit[j]].DV==Nodes[toVisit[j+1]].DV && toVisit[j]<toVisit[j+1]))
                {
                    flag=1;
                    Swap(toVisit[j],toVisit[j+1]);
                }
            }
        }
    }
    //first non visited
    for(i=0;i<MapSize;i++){
        if(Nodes[toVisit[i]].Visited==0){
            Next=toVisit[i];
            printf("%d",Nodes[toVisit[i]].Visited);
            printf("%d",Next);
            break;
        }
    }
    if(flag==0)
        return Next;
    else
        return -1;
}



void Swap(int *b,int *c)
{
    int x;
    x=b;
    b=c;
    c=x;
}
