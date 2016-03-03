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

    //Display Graph
#ifdef debug
    for(i=0; i<MapSize; i++)
    {
        for(k=0; k<4; k++)
        {
            printf("%d\t", Nodes[i].Neighbours[k]);
        }
        for(k=0; k<4; k++)
        {
            printf("%d\t", Nodes[i].Distance[k]);
            if(k==3)
            printf("\n");
        }
    }
#endif


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
        *(toVisit+i)=-1;

    //define distance value of neighbours and queue them. First without sorting, then handle sorting for each node added to the queue
    n=0;
    //iterate over the nodes, starting with the neighbours of the current node
    for(i=0; i<MapSize; i++)
    {
        //there are potentially four neighbouring nodes, so consider each side
        for(k=0; k<4; k++)
        {
            //if the neighbour exists, and if it has not yet been visited, find the distance value
            if(Nodes[Current].Neighbours[k]!=-1 && Nodes[Nodes[Current].Neighbours[k]].Visited==0 )
            {
                //if the calculated DV (current node DV+neighbour distance cost) is less than the neighbour node's DV, assign this new DV.
                if( Nodes[Nodes[Current].Neighbours[k]].DV > ( Nodes[Current].DV + Nodes[Current].Distance[k] ) )
                    Nodes[Nodes[Current].Neighbours[k]].DV = ( Nodes[Current].DV + Nodes[Current].Distance[k] );

                //The neighbouring node has not yet been visited, so we'll add it in a queue to be visit when it is its turn.
                if(Nodes[Nodes[Current].Neighbours[k]].Queued==0)
                {
                    *(toVisit+n)=Nodes[Current].Neighbours[k];
                    Nodes[Nodes[Current].Neighbours[k]].Queued=1;

                    n++;
                }
            }
        }
        //the current node's neighbours have all been assigned a distance value. Time to mark it as visited and move onto the next "Current" node.
        Nodes[Current].Visited=1;
        //The neighbours of this current node are marked for visitation. Now let's sort, in order to visit the lowest DV with the lowest indexes first
        SortQueue(Nodes,toVisit);
        //Because the toVisit array/queue is now sorted, we're confident that toVisit[i] is the next appropriate "current node"
        Current=*(toVisit+i);

        //repeat the loop until all nodes have been visited. By this algorithm's nature, MapSize is the number of iterations needed to visit each node.
    }


#ifdef debug
    printf("Sorted Queue:\n");

    for(n=0; n<MapSize-1; n++)
    {
        printf("Node: %d , DV: %d, Visited: %d\n", *(toVisit+n),Nodes[*(toVisit+n)].DV,Nodes[*(toVisit+n)].Visited);

    }
#endif


//THE DIJKSTRA PATH, USING THE PREVIOUS NODES, STARTING AT THE FINISH!
    PathLength=GetPath(Nodes,MapSize,Finish);
#ifdef debug
    printf("PathLength: %d\n",PathLength);
    printf("Shortest Path: \n");
    for(i=0; i<PathLength; i++)
    {
        printf("%d -> ",*(Path+i));
    }
#endif
    return 0;
}

void ReadNodes(NodeStruct *Nodes, int MapSize)
{
    i = 0;
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
        //initialise previous nodes with -1. This is important, because at the end, the only node with previous -1 will be the starting node, which is our indication when going from finish to start, using previous nodes.
        Nodes[j].Previous=-1;

#ifdef debug
        printf("%d, %d, %d\n",Nodes[j].Visited,Nodes[j].DV,Nodes[j].Previous);
#endif
    }
}

void SortQueue(NodeStruct *Nodes, int *toVisit)
{
    int j;
    for(j=0; j<MapSize-1; j++)
    {
        if(Nodes[*(toVisit+j)].DV>Nodes[*(toVisit+j+1)].DV)
            Swap((toVisit+j),(toVisit+j+1));
        else if(Nodes[*(toVisit+j)].DV==Nodes[*(toVisit+j+1)].DV && *(toVisit+j)>*(toVisit+j+1))
            Swap((toVisit+j),(toVisit+j+1));

    }
}


//Swap two integers
void Swap(int *b,int *c)
{
    int x;
    x=*b;
    b=c;
    c=&x;
}


int GetPath(NodeStruct *Nodes, int MapSize, int Finish)
{

    Node=Finish;

    PathLength=0; // count the number of nodes in path, excluding start node
    printf("Previous: %d\n",LowestDVNode(Nodes,Node));
    while(LowestDVNode(Nodes,Node)!=-1)
    {

        Node=LowestDVNode(Nodes,Node);
        Path[PathLength]=Node;
        PathLength++;
        printf("Path node: %d",Path[PathLength]);
    }

    return PathLength;
}

int LowestDVNode(NodeStruct *Nodes, int Node)
{
    LowestDV=999;
    for(k=0; k<4; k++)
    {
        printf("First Neighbour: %d\n",Nodes[Node].Neighbours[k]);
        if(Nodes[Node].Neighbours[k]!=-1)
        {
            if(Nodes[Nodes[Node].Neighbours[k]].DV<Nodes[Node].DV && Nodes[Nodes[Node].Neighbours[k]].DV < LowestDV)
            {
                LowestDV = Nodes[Nodes[Node].Neighbours[k]].DV;
                Nodes[Node].Previous=Nodes[Node].Neighbours[k];
                Node=Nodes[Node].Neighbours[k];
                printf("previous: %d\n",Nodes[Node].Previous);
            }
        }
    }
    if (LowestDV==999)
        return -1;
    else
        return Nodes[Node].Previous;
}
