#include "MapGeneration.h"
#include "World/Block.h"

#define TREE_HEIGHT 9
#define TREE_SIZE 5 //x/z size

enum TreeType {
    BasicTree = 0,
    FirTree,
    First = BasicTree,
    Last = FirTree,
    Size = Last + 1
};

__BLOCK_TYPE air = Block::Air;
__BLOCK_TYPE lg = Block::Log;
__BLOCK_TYPE lvs = Block::Leaves;

__BLOCK_TYPE TreeModels[TreeType::Size][TREE_HEIGHT][TREE_SIZE][TREE_SIZE] =
{
    //--------------------------------------------------------------
    //BasicTree
    {
        { // 0
            {air, air, air, air, air},
            {air, air, air, air, air},
            {air, air, lg, air, air},
            {air, air, air, air, air},
            {air, air, air, air, air},
        },
        { // 1
            {air, air, air, air, air},
            {air, air, air, air, air},
            {air, air, lg, air, air},
            {air, air, air, air, air},
            {air, air, air, air, air},
        },
        { // 2
            {air, lvs, lvs, lvs, air},
            {lvs, lvs, lvs, lvs, lvs},
            {lvs, lvs, lg, lvs, lvs},
            {lvs, lvs, lvs, lvs, lvs},
            {air, lvs, lvs, lvs, air},
        },
        { // 3
            {air, lvs, lvs, lvs, air},
            {lvs, lvs, lvs, lvs, lvs},
            {lvs, lvs, lg, lvs, lvs},
            {lvs, lvs, lvs, lvs, lvs},
            {air, lvs, lvs, lvs, air},
        },
        { // 4
            {air, lvs, lvs, lvs, air},
            {lvs, lvs, lvs, lvs, lvs},
            {lvs, lvs, lg, lvs, lvs},
            {lvs, lvs, lvs, lvs, lvs},
            {air, lvs, lvs, lvs, air},
        },
        { // 5
            {air, air, air, air, air},
            {air, lvs, lvs, lvs, air},
            {air, lvs, lvs, lvs, air},
            {air, lvs, lvs, lvs, air},
            {air, air, air, air, air},
        }
    },
    //--------------------------------------------------------------
    //FirTree
    {
        { // 0
            {air, air, air, air, air},
            {air, air, air, air, air},
            {air, air, lg, air, air},
            {air, air, air, air, air},
            {air, air, air, air, air},
        },
        { // 1
            {air, air, air, air, air},
            {air, air, air, air, air},
            {air, air, lg, air, air},
            {air, air, air, air, air},
            {air, air, air, air, air},
        },
        { // 2
            {air, lvs, lvs, lvs, air},
            {lvs, lvs, lvs, lvs, lvs},
            {lvs, lvs, lg, lvs, lvs},
            {lvs, lvs, lvs, lvs, lvs},
            {air, lvs, lvs, lvs, air},
        },
        { // 3
            {air, lvs, lvs, lvs, air},
            {lvs, lvs, lvs, lvs, lvs},
            {lvs, lvs, lg, lvs, lvs},
            {lvs, lvs, lvs, lvs, lvs},
            {air, lvs, lvs, lvs, air},
        },
        { // 4
            {air, air, lvs, air, air},
            {air, lvs, lvs, lvs, air},
            {lvs, lvs, lg, lvs, lvs},
            {air, lvs, lvs, lvs, air},
            {air, air, lvs, air, air},
        },
        { // 5
            {air, air, lvs, air, air},
            {air, lvs, lvs, lvs, air},
            {lvs, lvs, lg, lvs, lvs},
            {air, lvs, lvs, lvs, air},
            {air, air, lvs, air, air},
        },
        { // 6
            {air, air, air, air, air},
            {air, air, lvs, air, air},
            {air, lvs, lg, lvs, air},
            {air, air, lvs, air, air},
            {air, air, air, air, air},
        },
        { // 7
            {air, air, air, air, air},
            {air, air, lvs, air, air},
            {air, lvs, lvs, lvs, air},
            {air, air, lvs, air, air},
            {air, air, air, air, air},
        },
        { // 8
            {air, air, air, air, air},
            {air, air, air, air, air},
            {air, air, lvs, air, air},
            {air, air, air, air, air},
            {air, air, air, air, air},
        }
    }
    //--------------------------------------------------------------
};