#include <stdio.h>
#include "redis.h"
#include "cluster.h"


void clusterInit(){
   int size =  sizeof(clusterState);
   printf("clusterState struct size=%d\n", size);
}

int main(){
    clusterInit();
}