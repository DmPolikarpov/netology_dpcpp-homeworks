#ifndef SQL_QUERY_BUILDER_FULL_H
#define SQL_QUERY_BUILDER_FULL_H

#include <string>
#include <vector>
#include <map>
#include <sstream>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns;
    std::string table;
    std::vector<std::pair<std::string, std::string>> where_conditions;

public:

    // set one column
    SqlSelectQueryBuilder& AddColumn(const std::string& column) 
    {
        columns.push_back(column);
        return *this;
    }

    // set several columns
    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns_list) noexcept 
    {
        for (const auto& column : columns_list) 
        {
            columns.push_back(column);
        }
        return *this;
    }

    // set table
    SqlSelectQueryBuilder& AddFrom(const std::string& table_name) 
    {
        table = table_name;
        return *this;
    }

    // set one WHERE
    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) 
    {
        where_conditions.push_back({ column, value });
        return *this;
    }

    // set several WHERE from map
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept 
    {
        for (auto it = kv.begin(); it != kv.end(); ++it) 
        {
            where_conditions.push_back({ it->first, it->second });
        }
        return *this;
    }

    // build full query
    std::string BuildQuery() const 
    {
        std::ostringstream query;

        // process SELECT
        query << "SELECT ";
        if (columns.empty()) {
            query << "*";
        }
        else {
            for (size_t i = 0; i < columns.size(); ++i) {
                if (i > 0) query << ", ";
                query << columns[i];
            }
        }

        // process FROM
        query << " FROM " << table;

        // process WHERE
        if (!where_conditions.empty()) {
            query << " WHERE ";
            for (size_t i = 0; i < where_conditions.size(); ++i) {
                if (i > 0) query << " AND ";
                query << where_conditions[i].first << "=" << where_conditions[i].second;
            }
        }

        query << ";";
        return query.str();
    }
};

#endif
