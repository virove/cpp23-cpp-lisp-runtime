#include <gtest/gtest.h>

#include "defines.h"
#include "cell_adaptor.h"

TEST(CellAdaptorText, IteratorForEach) {
foundation::mem::CellAtom nodeNIL(2 );

    foundation::mem::CellNumber nodeNumber3(3);
    foundation::mem::CellList nodeList3(&nodeNumber3, &nodeNIL);


    foundation::mem::CellNumber nodeNumber2(2);
    foundation::mem::CellList nodeList2(&nodeNumber2, &nodeList3);

    foundation::mem::CellNumber nodeNumber1(1);
    foundation::mem::CellList nodeList1(&nodeNumber1, &nodeList2);

    lisp_runtime::CellAdaptor lListAdaptor(&nodeList1);

    int expected[]={1,2,3};
    auto iteraror_expected = std::begin(expected);
    for(auto item : lListAdaptor){
        EXPECT_EQ(*iteraror_expected, item->head_->number_);
        iteraror_expected++;
    }
}



TEST(CellAdaptorText, IteratorHitsEnd) {
    foundation::mem::CellNumber nodeNumber1(1);
    foundation::mem::CellAtom nodeAtom(2 );
    foundation::mem::CellList nodeList(&nodeNumber1, &nodeAtom);
    lisp_runtime::CellAdaptor lListAdaptor(&nodeList);
    EXPECT_TRUE(lListAdaptor.begin() != lListAdaptor.end());
    auto iterator = lListAdaptor.begin();
    iterator++;
    EXPECT_TRUE(iterator == lListAdaptor.end());
    iterator++;
    EXPECT_TRUE(iterator == lListAdaptor.end());
}