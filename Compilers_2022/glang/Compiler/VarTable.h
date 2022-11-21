#pragma once

#include <unordered_map>
#include <vector>
#include <stack>

namespace ezg
{
    /// @brief Implements a tree of scopes.
    /// @tparam Key_t - id type. 
    /// The type must meet the 'key' requirements in std::unordered_map.
    /// @tparam Val_t - value type.
    template <typename Key_t, typename Val_t>
    class ScopeTable final
    {
        struct Node_t final
        {
            size_t m_parent = -1;
            std::unordered_map<Key_t, Val_t> m_data;
        }; // class Node_t

        std::vector<Node_t> m_tables;
        std::stack<size_t> m_scopeIdStore;

    public:
        /// @brief Entering the table with the id number.
        /// The id is placed at the top of the stack. When exit_scope is called, 
        /// the table will return to the previous scope (and not to the parent of cur scope).
        /// @param id - table identifier.
        void entryScope(size_t id) { m_scopeIdStore.push(id); }

        /// @brief In stack order returns to the previous table.
        void exitCurScope() { m_scopeIdStore.pop(); }

        /// @brief 
        /// @return identifier of cur table.
        size_t getCurTableId() const { return m_scopeIdStore.top(); }

        /// @brief Crete new scope table.
        /// @param prevId - parent scope. All objects from prevId table are visible
        /// from new table. If prevId is -1, than new table is not conneted to any old table.
        /// @return id of new table.
        size_t createTable(size_t prevId = -1)
        {
            if (prevId != -1 && prevId >= m_tables.size())
            {
                throw std::out_of_range("prevId greaater than number of tables!");
            }
            size_t newId = m_tables.size();

            Node_t newNode;
            newNode.m_parent = prevId;

            m_tables.push_back(newNode);
            return newId;
        }

        /// @brief return data of VISIBLE object with 'id'
        Val_t &access(const Key_t &id)
        {
            size_t cur_id = m_scopeIdStore.top();
            while (cur_id != -1)
            {
                auto &&table = m_tables.at(cur_id);
                if (table.m_data.count(id))
                {
                    return table.m_data.at(id);
                }
                cur_id = table.m_parent;
            }
            throw std::runtime_error("Undeclared id!");
        }

        /// @brief return true if 'id' is visible at 'idTable' table.
        /// If there is not 'id' at 'idTable' and 'idTable' connected to 'prevId' (see createTable),
        /// than algorithm search 'id' at 'prevId'.
        bool isDeclared(size_t idTable, const Key_t &id) const
        {
            bool result = false;
            while (idTable != -1 && !result)
            {
                auto &&table = m_tables.at(idTable);
                result = table.m_data.count(id);
                idTable = table.m_parent;
            }
            return result;
        }

        /// @brief Declare new object at certain scope
        /// @param idTable - scop where object were declared
        /// @param idElem - identifier of new element
        /// @param data - value of new element
        void declare(size_t idTable, const Key_t &idElem, const Val_t &data)
        {
            m_tables[idTable].m_data[idElem] = data;
        }
    }; // class ScopeTable

} // namespace ezg