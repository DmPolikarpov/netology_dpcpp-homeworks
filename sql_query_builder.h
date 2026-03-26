#ifndef SQL_QUERY_BUILDER_H
#define SQL_QUERY_BUILDER_H

#include <string>
#include <vector>
#include <sstream>

class SqlSelectQueryBuilder
{
private:
    std::vector<std::string> columns;
    std::string table;
    std::vector<std::pair<std::string, std::string>> where_conditions;

public:

    SqlSelectQueryBuilder& AddColumn(const std::string& column)
    {
        columns.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table_name)
    {
        table = table_name;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value)
    {
        where_conditions.push_back({ column, value });
        return *this;
    }

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
