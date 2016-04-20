//========================================
//
//  Dijkstra.c | C Implementation of the Dijkstra Algorithm
//  Authors: Abdil Kaya & Robin Janssens
//
//========================================


#include "dijkstra.h"
#define debug

//Set Initial DV, Visited and Previous values for all nodes.
int Dijkstra(NodeStruct *Nodes, int MapSize, int Start, int Finish)
{

//Taken from header to avoid multiple definition of .. .. ..
MapSize=11;
Graph[10]="Graph.dat";
LowestDV=999;
//


    //Set the current node to visit to Start
    Current=Start;
#ifdef debug
    printf("Init dijkstra\n");
#endif
    InitDijkstra(Nodes, MapSize, Start);

    for (i=0; i <MapSize; ++i)
        *(toVisit+i)=-1;
    *(toVisit+0)=Start;


    //define distance value of neighbours and queue them. First without sorting, then handle sorting for each node added to the queue
    n=0;
    //iterate over the nodes, starting with the neighbours of the current node
    for(i=0; i<MapSize; i++)
    {

        //there are potentially four neighbouring nodes, so consider each side
        for(k=0; k<4; k++)
        {
            //if the neighbour exists, and if it has not yet been visited, find the distance value
            if(Nodes[Current].Neighbours[k]!=-1 && Nodes[Nodes[Current].Neighbours[k]].Visited==0)
            {
                //if the calculated DV (current node DV+neighbour distance cost) is less than the neighbour node's DV, assign this new DV.
                if( Nodes[Nodes[Current].Neighbours[k]].DV > ( Nodes[Current].DV + Nodes[Current].Distance[k] ) )
                {
                    Nodes[Nodes[Current].Neighbours[k]].DV = ( Nodes[Current].DV + Nodes[Current].Distance[k] );
                    //Because this current node's DV is lower than the neighbour's DV, we can safely assume this node is the neighbouring node's "previous" node with respect to the start node.
                    Nodes[Nodes[Current].Neighbours[k]].Previous=Current;
                    //Defining the next node is dependent on the finish node, therefore we'll cover that in the DefPath function.
#ifdef debug
                    printf("\nnode: %d, previous node: %d",Nodes[Current].Neighbours[k],Nodes[Nodes[Current].Neighbours[k]].Previous);
#endif
                }
                //The neighbouring node has not yet been visited, so we'll add it in a queue and mark it to be visited
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

        //The neighbours of this current node are marked for visitation. Now let's sort, in order to visit the lowest DV nodes with the lowest indexes first
        //Imagine an expanding spiderweb, but without the sequential build up
        Current=SortQueue(Nodes,toVisit);

        //repeat the loop MapSize times in order to visit every node. Each iteration will mark exactly 1 node as visited.
    }


#ifdef debug
    printf("\n\nSorted Queue:\n");

    for(n=0; n<MapSize-1; n++)
    {
        printf("Node: %d , DV: %d, Visited: %d\n", *(toVisit+n),Nodes[*(toVisit+n)].DV,Nodes[*(toVisit+n)].Visited);

    }
#endif
//THE DIJKSTRA PATH, USING THE PREVIOUS NODES, STARTING AT THE FINISH!
    PathLength=DefPath(Nodes,MapSize,Start,Finish);
#ifdef debug
    printf("\nPathLength: %d\n",PathLength);
    printf("Shortest Path (Reversed): \n");
    for(i=0; i<PathLength+1; i++)
    {
        printf("-> %d ",(int)*(Path+i));
    }
#endif
    return PathLength;
}



//Read Graph/Nodes and their Members' values from a data file
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
#ifdef debug
        printf("\n");
#endif
    }
    else
        printf("File does not exist.");
}



//Set Initial DV, Visited, Previous and Next values for all nodes.
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
        Nodes[j].Next=-1;
        //Robots will move forward by default. This is useful for the DefDirs function which will disregard the first node and make the robot move forward by default.
        Nodes[j].NextRelDir=2;
        Nodes[j].NextAbsDir=-1;


#ifdef debug
        printf("%d, %d, %d\n",Nodes[j].Visited,Nodes[j].DV,Nodes[j].Previous);
#endif
    }
}



//Sorts the "toVisit[]" queue
int SortQueue(NodeStruct *Nodes, int *toVisit)
{
    int NextVisit;
#ifdef debug
    printf("\nSorting: \n");
#endif
    int j=0;
    //for(j=0; j<MapSize-1; j++)

    while(*(toVisit+j)!=-1 && *(toVisit+j+1)!=-1)
    {
#ifdef debug
        printf("index %d: , node: %d -> DV: %d ;;; index %d , node: %d -> DV: %d \n",j,*(toVisit+j),Nodes[*(toVisit+j)].DV, j+1,*(toVisit+j+1),Nodes[*(toVisit+j+1)].DV);
#endif

        if(Nodes[*(toVisit+j)].DV>Nodes[*(toVisit+j+1)].DV)
        {
#ifdef debug
            printf("index %d: , node: %d -> DV: %d bigger than index %d , node: %d -> DV: %d \n",j,*(toVisit+j),Nodes[*(toVisit+j)].DV, j+1,*(toVisit+j+1),Nodes[*(toVisit+j+1)].DV);
#endif
            Swap((toVisit+j),(toVisit+j+1));
        }
        else if(Nodes[*(toVisit+j)].DV==Nodes[*(toVisit+j+1)].DV && *(toVisit+j)>*(toVisit+j+1))
        {
#ifdef debug
            printf("index %d: , NODE: %d -> DV: %d bigger than index %d , NODE: %d -> DV: %d \n",j,*(toVisit+j),Nodes[*(toVisit+j)].DV, j+1,*(toVisit+j+1),Nodes[*(toVisit+j+1)].DV);
#endif
            Swap((toVisit+j),(toVisit+j+1));
        }
        j++;
    }

    for(j=MapSize; j>=0; j--)
    {
        if(Nodes[*(toVisit+j)].Visited==0)
            NextVisit=*(toVisit+j);
    }
#ifdef debug
    printf("Sorted properly; \n");
#endif

    return NextVisit;
}



//Swap two integers
void Swap(int *b,int *c)
{
    int x;
    x=*b;
    *b=*c;
    *c=x;
}



//Puts the Dijkstra-defined path node indexes in an integer array //could be optimized with dynamic array allocation
int DefPath(NodeStruct *Nodes, int MapSize, int Start, int Finish)
{

    Node=Finish;

    PathLength=0; // count the number of nodes in path, excluding start node
    while(Node!=-1)
    {
        //Assigning the nodes to the Path array, whilst keeping track of the Path's length.
        *(Path+PathLength)=Node;
        //This is the most convenient place to define the Next node, which is dependent on the Finish node, whereas the previous node was dependent on the Start node.
        Nodes[Nodes[Node].Previous].Next=Node;
#ifdef debug
        printf("Previous Node: %d, Previous Node's Next node: %d \n", Nodes[Node].Previous,Nodes[Nodes[Node].Previous].Next);
#endif
        Node=Nodes[Node].Previous;

        PathLength++;
        //INFO
        // The Developer can use Nodes[Start].Next while .Next != -1 to go from Start to Finish. The Developer can also use the reverse of the Path array pointer formed in this function
    }
    DefDirs(Nodes,MapSize,Start,Finish);
    PathLength--;
    return PathLength;
}



//Uses the Next/Previous nodes to find the Next Absolute and Relative Direction for the robot's movements
void DefDirs(NodeStruct *Nodes, int MapSize, int Start, int Finish)
{
    Current = Start;
    while(Nodes[Current].Next!=-1)
    {
        //NextAbsDir found through the use of Neighbour index
        for(k=0; k<4; k++)
        {
            if(Nodes[Current].Next == Nodes[Current].Neighbours[k])
            {
                Nodes[Current].NextAbsDir=k;
            }
        }

        //PreviousAbsDir temporarily stored in a pad variable in order to find the NextRelDir
        //PreviousAbsDir and NextAbsDir are on the SAME node.
        //"pad" (=PreviousAbsDir) defines the node ENTRY direction
        int pad;
        for(k=0; k<4; k++)
        {
            if(Nodes[Current].Previous!=-1 && Nodes[Current].Previous == Nodes[Current].Neighbours[k])
            {
                pad=k;
            }
        }


        int nad = Nodes[Current].NextAbsDir;
        //NextRelDir found through the use of "pad"(~ENTRY direction) and "nad" (~NextAbsDir~EXIT direction)
        //The following statements using the modulus function applies.
        //Modulus function is an additional function, because the C-modulus operator is actually a remainder operator
        if(Current!=Finish && Current != Start)
        {

            switch(pad)
            {
            case 0:
                Nodes[Current].NextRelDir=nad;
                break;

            case 1:
                Nodes[Current].NextRelDir=mod((nad-pad),4);
                break;

            case 2:
                Nodes[Current].NextRelDir=mod((nad+pad),4);
                break;

            case 3:
                Nodes[Current].NextRelDir=mod((nad-pad),4);
                break;

            default :
                Nodes[Current].NextRelDir=-1;
            }

        }
#ifdef debug
        printf("\nNode: %d, NAD: %d, NRD: %d\n",Current, Nodes[Current].NextAbsDir,Nodes[Current].NextRelDir);
#endif
        Current=Nodes[Current].Next;
    }
}



//true modulo operator (using the C _remainder_ operator)
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}
