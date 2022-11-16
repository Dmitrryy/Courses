#pragma once

#include <unordered_map>
#include <vector>
#include <cassert>
#include <optional>
#include <stack>
#include <memory>

namespace ezg
{
    template< typename Key_t, typename Val_t >
    class ScopeTable final
    {

        class Node_t final
        {
            Node_t *m_parent = nullptr;
            std::vector<Node_t *> m_children;

            std::unordered_map<Key_t, Val_t> m_data;

        public:
            void setParent(Node_t *pNode) { m_parent = pNode; }

            void addChild(Node_t *pNode)
            {
                pNode->setParent(this); /**/
                m_children.push_back(pNode);
            }

            void declare(const Key_t &id, const Val_t &data)
            {
                assert(m_data.count(id) == 0);
                m_data[id] = std::move(data);
            }

            Val_t &access(const Key_t &id)
            {
                if (m_data.count(id)) {
                    return access_cur_(id);
                }

                if (m_parent != nullptr)
                {
                    return m_parent->access(id);;
                }

                throw std::runtime_error("Undeclared id!");
            }

            bool isDeclared(const Key_t &id) const
            {
                bool res = isDeclared_cur_(id);
                if(!res && m_parent != nullptr) {
                    res = m_parent->isDeclared(id);
                }
                return res;
            }

        private:
            Val_t &access_cur_(const Key_t &id)
            {
                return m_data.at(id);
            }

            bool isDeclared_cur_(const Key_t &id) const
            {
                return m_data.count(id);
            }
        }; // class Node_t

        std::vector<Node_t *> m_tables;
        std::stack<size_t> m_scopeIdStore;

    public:
        ScopeTable() = default;

        // not supported
        ScopeTable(const ScopeTable &) = delete;
        const ScopeTable &operator=(const ScopeTable &) = delete;
        ScopeTable(ScopeTable &&) = delete;
        const ScopeTable &operator=(ScopeTable &&) = delete;

        ~ScopeTable()
        {
            for (const auto &pN : m_tables)
            {
                delete pN;
            }
        }

        void entryScope(size_t id)
        {
            assert(id < m_tables.size());
            m_scopeIdStore.push(id);
        }
        void exitCurScope()
        {
            m_scopeIdStore.pop();
        }

        size_t getCurTableId() const noexcept { return m_scopeIdStore.top(); }

        size_t createTable(size_t prevId)
        {
            assert(prevId < m_tables.size());
            size_t newId = m_tables.size();
            auto *newNode = new Node_t();
            m_tables.push_back(newNode);

            newNode->setParent(m_tables[prevId]);
            m_tables[prevId]->addChild(newNode);

            return newId;
        }

        size_t createTable()
        {
            size_t newId = m_tables.size();
            auto *newNode = new Node_t();

            m_tables.push_back(newNode);

            return newId;
        }

        Val_t& access(const Key_t &id) const
        {
            return m_tables[m_scopeIdStore.top()]->access(id);
        }

        bool isDeclared(size_t idTable, const Key_t &id) const {
            return m_tables.at(idTable)->isDeclared(id);
        }

        void declare(size_t idTable, const Key_t &idElem, const Val_t &data) { m_tables[idTable]->declare(idElem, data); }
    }; // class ScopeTable

} // namespace ezg