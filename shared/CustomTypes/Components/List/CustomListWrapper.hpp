#pragma once
#include "CustomCellListTableData.hpp"
#include <functional>

namespace QuestUI {
    class CustomListWrapper {
        public:
            /// @brief gets the length of the data (= amt of cells)
            virtual int GetDataSize() = 0;
            /// @brief a method to call for setting up the table
            /// use this to add to the selected / highlighted / neither groups of the cell, so you can get different behaviour
            virtual void SetupCell(QuestUI::CustomCellTableCell* createdCell, int idx) = 0;
            /// @brief what to do when a cell with idx is selected
            virtual void OnCellSelected(QuestUI::CustomCellTableCell* cell, int idx) = 0;
            /// @brief a dotr so you can clear up your data if needed
            virtual ~CustomListWrapper() {};
    };

    class LambdaListWrapper : public CustomListWrapper {
        public:
            /// @brief a type that wraps 4 lambdas to provide the same functionality as just inheriting the class and setting up a proper instance
            LambdaListWrapper(std::function<int(LambdaListWrapper*)> getDataSize, std::function<void(LambdaListWrapper*, QuestUI::CustomCellTableCell*, int)> setupCell, std::function<void(LambdaListWrapper*, QuestUI::CustomCellTableCell*, int)> onCellSelected, std::function<void(LambdaListWrapper*)> dtor) : getDataSize(getDataSize), setupCell(setupCell), onCellSelected(onCellSelected), dtor(dtor) {}; 

            std::function<int(LambdaListWrapper*)> getDataSize;
            std::function<void(LambdaListWrapper*, QuestUI::CustomCellTableCell*, int)> setupCell;
            std::function<void(LambdaListWrapper*, QuestUI::CustomCellTableCell*, int)> onCellSelected;
            std::function<void(LambdaListWrapper*)> dtor;

            int GetDataSize() { return getDataSize(this); };
            void SetupCell(QuestUI::CustomCellTableCell* createdCell, int idx) { setupCell(this, createdCell, idx); };
            void OnCellSelected(QuestUI::CustomCellTableCell* cell, int idx) { onCellSelected(this, cell, idx); };
            ~LambdaListWrapper() { dtor(this); };
    };

    template<typename T, typename vec = std::vector<T>>
    class VectorListWrapper : public CustomListWrapper {
        vec data;
        public:
            /// @brief a type that wraps a vector to make you need to define less methods;
            VectorListWrapper(vec data) : data(data) {};
            int GetDataSize() { return data.size(); };
            void SetupCell(QuestUI::CustomCellTableCell* createdCell, int idx) { SetupVectorCell(createdCell, data[idx]); };
            void virtual SetupVectorCell(QuestUI::CustomCellTableCell* createdCell, const T& data) = 0;
            virtual void OnCellSelected(QuestUI::CustomCellTableCell* cell, int idx) = 0;
            virtual ~VectorListWrapper() = 0;
    };

    template<typename T, typename arr = Array<T>*>
    class ArrayListWrapper : public CustomListWrapper {
        arr data;
        public:
            /// @brief a type that wraps an array to make you need to define less methods;
            ArrayListWrapper(arr data) : data(data) {};
            int GetDataSize() { return data->Length(); };
            void SetupCell(QuestUI::CustomCellTableCell* createdCell, int idx) { SetupVectorCell(createdCell, data->values[idx]); };
            void virtual SetupArrayCell(QuestUI::CustomCellTableCell* createdCell, const T& data) = 0;
            virtual void OnCellSelected(QuestUI::CustomCellTableCell* cell, int idx) = 0;
            virtual ~ArrayListWrapper() = 0;
    };

        template<typename T, typename list = List<T>*>
    class ListListWrapper : public CustomListWrapper {
        list data;
        public:
            /// @brief a type that wraps a list to make you need to define less methods;
            ListListWrapper(list data) : data(data) {};
            int GetDataSize() { return data->get_Count(); };
            void SetupCell(QuestUI::CustomCellTableCell* createdCell, int idx) { SetupVectorCell(createdCell, data->items->values[idx]); };
            void virtual SetupListCell(QuestUI::CustomCellTableCell* createdCell, const T& data) = 0;
            virtual void OnCellSelected(QuestUI::CustomCellTableCell* cell, int idx) = 0;
            virtual ~ListListWrapper() = 0;
    };
}
