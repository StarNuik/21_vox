#pragma once
// #include "MapGeneration.h"
#include "World/Block.h"

#define TREE_HEIGHT 10
#define TREE_SIZE 7 //x/z size


class Trees
{
private:
    const __BLOCK_TYPE air = Block::Air;
    const __BLOCK_TYPE lg = Block::Log;
    const __BLOCK_TYPE lvs = Block::Leaves;
    const __BLOCK_TYPE cts = Block::Cactus;
public:
   enum TreeType
   {
        BasicTree = 0,
        OakTreeTypeOne,
        OakTreeTypeTwo,
        SpruceTreeTypeOne,
        SpruceTreeTypeTwo,
        Сactus,
        Nothing,
        First = BasicTree,
        Last = Сactus,
        Size = Last + 1
    };

    const __BLOCK_TYPE TreeModels[TreeType::Size][TREE_HEIGHT][TREE_SIZE][TREE_SIZE] =
    {
        //--------------------------------------------------------------
        //BasicTree
        {
            { // 0
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 1
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 2
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 3
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 4
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 5
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            }
        },
        //--------------------------------------------------------------
        //OakTreeTypeOne
        {
            { // 0
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 1
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 2
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 3
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 4
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 5
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lg,   lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 6
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
        },
        //--------------------------------------------------------------
        //OakTreeTypeTwo
        {
            { // 0
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 1
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lg,   lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 2
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 3
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 4
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 5
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
        },
        //--------------------------------------------------------------
        //SpruceTreeTypeOne
        {
            { // 0
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 1
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 2
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lg,   lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 3
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 4
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lg,  lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 5
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 6
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 7
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 8
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            }
        },
        //--------------------------------------------------------------
        //SpruceTreeTypeTwo
        {
            { // 0
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lg,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 1
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lg,   lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 2
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {lvs,  lvs,  lvs,  lvs,  lvs,  lvs,  lvs},
                {lvs,  lvs,  lvs,  lg,   lvs,  lvs,  lvs},
                {lvs,  lvs,  lvs,  lvs,  lvs,  lvs,  lvs},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air}
            },
            { // 3
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 4
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lg,   lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 5
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  lvs,  lvs,  lg,   lvs,  lvs,  air},
                {air,  lvs,  lvs,  lvs,  lvs,  lvs,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 6
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lg,   lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 7
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 8
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  lvs,  lvs,  lvs,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 9
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  lvs,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            }
        },
        //--------------------------------------------------------------
         // Cactus
        {
            { // 0
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  cts,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 1
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  cts,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            },
            { // 2
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  cts,   air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air},
                {air,  air,  air,  air,  air,  air,  air}
            }
        }
        //--------------------------------------------------------------
    };
};
