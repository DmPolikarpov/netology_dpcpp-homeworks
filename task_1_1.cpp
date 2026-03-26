#include <iostream>

#include "sql_query_builder.h"

int main()
{

    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    std::string result = query_builder.BuildQuery();
    std::cout << result << std::endl;

    if (result == "SELECT name, phone FROM students WHERE id=42 AND name=John;") {
        std::cout << "Query built correctly!" << std::endl;
    }

	return EXIT_SUCCESS;
}