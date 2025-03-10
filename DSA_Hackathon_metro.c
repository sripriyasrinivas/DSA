#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATIONS 100
#define MAX_LINES 10
#define MAX_QUEUE 1000

typedef struct Edge 
{
    char src[20];
    char dest[20];
    int cost;
} Edge;

typedef struct Line 
{
    char name[20];
    int numStations;
    char stations[MAX_STATIONS][20];
} Line;

typedef struct Station 
{
    char name[20];
    int numLines;
    char lines[MAX_LINES][20];
    Edge adj[MAX_STATIONS];
    int numAdj;
} Station;

typedef struct QueueNode 
{
    char station[20];
    int cost;
    char path[MAX_STATIONS][20];
    int pathLen;
} QueueNode;

// Function to find a station by name
Station* findStationByName(char *name, Station stations[], int SC) 
{
    for (int i = 0; i < SC; i++) 
    {
        if (strcmp(stations[i].name, name) == 0) 
        {
            return &stations[i];
        }
    }
    return NULL;
}

// Function to find or create a station
int findOrCreateStation(char *name, Station stations[], int *SC) 
{
    Station* station = findStationByName(name, stations, *SC);
    if (station != NULL) 
    {
        return station - stations;//for getting index placeholder
    }
    strcpy(stations[*SC].name, name);
    stations[*SC].numLines = 0;
    stations[*SC].numAdj = 0;
    return (*SC)++;
}

// Function to add an edge (route) between two stations 
//since undirected both the ways should be possible
void addEdge(char *src, char *dest, int cost, Station stations[], int *SC)
{
    int srci = findOrCreateStation(src, stations, SC);
    int desti = findOrCreateStation(dest, stations, SC);

    strcpy(stations[srci].adj[stations[srci].numAdj].src, src);
    strcpy(stations[srci].adj[stations[srci].numAdj].dest, dest);
    stations[srci].adj[stations[srci].numAdj].cost = cost;
    stations[srci].numAdj++;

    strcpy(stations[desti].adj[stations[desti].numAdj].src, dest);
    strcpy(stations[desti].adj[stations[desti].numAdj].dest, src);
    stations[desti].adj[stations[desti].numAdj].cost = cost;
    stations[desti].numAdj++;
}

// Function to add a metro line with its stations
void addLine(char *name, int numStations, char stationNames[][20], Line lines[], int *lineCount, Station stations[], int *SC) 
{
    strcpy(lines[*lineCount].name, name);
    lines[*lineCount].numStations = numStations;
    for (int i = 0; i < numStations; i++) 
    {
        strcpy(lines[*lineCount].stations[i], stationNames[i]);
        int stationi = findOrCreateStation(stationNames[i], stations, SC);
        strcpy(stations[stationi].lines[stations[stationi].numLines++], name);
    }
    (*lineCount)++;
}

// Function to check if a station is a terminal for a line
int is_Terminal(Station *station, char *lineName, Station stations[], int SC) 
{
    int connectionsOnLine = 0;

    for (int i = 0; i < station->numAdj; i++) 
    {
        Station* adj = findStationByName(station->adj[i].dest, stations, SC);
        if (adj) 
        {
            for (int j = 0; j < adj->numLines; j++) 
            {
                if (strcmp(adj->lines[j], lineName) == 0) 
                {
                    connectionsOnLine++;
                    break;
                }
            }
        }
    }

    return connectionsOnLine <= 1;
}

// Function to display terminal stations of all lines
void displayTerminalStations(Line lines[], int lineCount, Station stations[], int SC) 
{
    for (int i = 0; i < lineCount; i++) 
    {
        printf("Terminals for %s: ", lines[i].name);
        int found = 0;

        for (int j = 0; j < lines[i].numStations; j++) 
        {
            Station *station = findStationByName(lines[i].stations[j], stations, SC);
            if (station != NULL && is_Terminal(station, lines[i].name, stations, SC)) 
            {
                printf("%s ", station->name);
                found = 1;
            }
        }

        if (!found) 
        {
            printf("No terminals found");
        }
        printf("\n");
    }
}

// Function to display station information
void displayStationInfo(char *stationName, Station stations[], int SC) 
{
    Station* station = findStationByName(stationName, stations, SC);
    if (station == NULL) 
    {
        printf("Station %s not found\n", stationName);
        return;
    }
    printf("Station Name: %s\n", station->name);
    printf("Lines: ");
    for (int j = 0; j < station->numLines; j++) 
    {
        printf("%s ", station->lines[j]);
    }
    printf("\nAdjacent Stations:\n");
    for (int j = 0; j < station->numAdj; j++) 
    {
        printf("%s, Cost: %d\n", station->adj[j].dest, station->adj[j].cost);
    }
}

void displayIntersections(Station stations[], int SC) 
{
    for (int i = 0; i < SC; i++) 
    {
        if (stations[i].numLines > 1) 
        {
            printf("%s, %d\n", stations[i].name, stations[i].numLines);
        }
    }
}

// Depth-First Search (DFS) for finding all routes between source and destination
void dfs(char *src, char *dest, int visited[], int path[], int pathi, int pathCost, int *foundPaths, Station stations[], int stationCount) 
{
    int srci = findStationByName(src, stations, stationCount) - stations;
    Station* srcStation = &stations[srci];

    visited[srci] = 1;
    path[pathi] = srci;
    pathi++;

    if (strcmp(src, dest) == 0) 
    {
        (*foundPaths)++;
        printf("Path %d: ", *foundPaths);
        for (int i = 0; i < pathi; i++)
        {
            printf("%s", stations[path[i]].name);
            if (i < pathi - 1) printf(" -> ");
        }
        printf(", Cost: %d\n", pathCost);
    } 
    else 
    {
        for (int i = 0; i < srcStation->numAdj; i++) 
        {
            int adji = findStationByName(srcStation->adj[i].dest, stations, stationCount) - stations;
            if (!visited[adji]) 
            {
                dfs(srcStation->adj[i].dest, dest, visited, path, pathi, pathCost + srcStation->adj[i].cost, foundPaths, stations, stationCount);
            }
        }
    }

    visited[srci] = 0;
}

// Function to find all routes from source to destination
void findRoutes(char *src, char *dest, Station stations[], int SC) 
{
    int visited[MAX_STATIONS] = {0};
    int path[MAX_STATIONS];
    int pathIdx = 0;
    int foundPaths = 0;
    dfs(src, dest, visited, path, pathIdx, 0, &foundPaths, stations, SC);
}

// Breadth-First Search (BFS) to find the nearest intersection(shortest path) (station with multiple lines)
void findNearestIntersection(char *src, Station stations[], int SC) 
{
    QueueNode queue[MAX_QUEUE];
    int front = 0, rear = 0;
    QueueNode sNode = {.cost = 0};
    strcpy(sNode.station, src);
    sNode.pathLen = 0;
    queue[rear++] = sNode;

    while (front < rear) 
    {
        QueueNode cNode = queue[front++];
        Station* cStation = findStationByName(cNode.station, stations, SC);

        // Check if the current station is an intersection (more than one line)
        if (cStation->numLines > 1) 
        {
            printf("Nearest Intersection: %s, Cost: %d\n", cNode.station, cNode.cost);
            printf("Path: ");
            for (int i = 0; i < cNode.pathLen; i++) 
            {
                printf("%s ", cNode.path[i]);
            }
            printf("\n");
            return;
        }
        for (int i = 0; i < cStation->numAdj; i++) 
        {
            QueueNode nextNode = cNode;
            strcpy(nextNode.station, cStation->adj[i].dest);
            nextNode.cost += cStation->adj[i].cost;
            strcpy(nextNode.path[nextNode.pathLen], cStation->adj[i].dest);
            nextNode.pathLen++;
            queue[rear++] = nextNode;
        }
    }

    printf("No intersection found.\n");
}

int main() 
{
    Station stations[MAX_STATIONS];
    int stationCount = 0;
    Line lines[MAX_LINES];
    int lineCount = 0;
    int n, m;
    printf("Enter the number of edges: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        char src[20], dest[20];
        int cost;
        scanf("%s %s %d", src, dest, &cost);
        addEdge(src, dest, cost, stations, &stationCount);
    }
    scanf("%d", &m);//input for no_of_lines
    for (int i = 0; i < m; i++) {
        char lineName[20];
        int numStations;
        scanf("%s %d", lineName, &numStations);
        char stationNames[numStations][20];
        for (int j = 0; j < numStations; j++) {
            scanf("%s", stationNames[j]);
        }
        addLine(lineName, numStations, stationNames, lines, &lineCount, stations, &stationCount);
    }
    
    printf("For displaying Station information, terminal stations and intersections\n");
    char stationName[20];
    printf("Enter station name: ");
    scanf("%s", stationName);
    displayStationInfo(stationName, stations, stationCount);
    displayTerminalStations(lines, lineCount, stations, stationCount);
    displayIntersections(stations, stationCount);
    char src[20], dest[20];
    printf("For finding routes between two stations\n");
    printf("Enter source station: ");
    scanf("%s", src);
    printf("Enter destination station: ");
    scanf("%s", dest);
    findRoutes(src, dest, stations, stationCount);
    printf("For finding the nearest intersection\n");
    printf("Enter source station: ");
    scanf("%s", src);
    findNearestIntersection(src, stations, stationCount);
    printf("Exiting program.\n");
    return 0;
}
