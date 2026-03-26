#include <iostream>
#include "sql_query_builder_full.h"

int main() 
{
    //  AddColumns
    SqlSelectQueryBuilder query_builder1;
    query_builder1.AddColumns({ "id", "name", "email", "phone" });
    query_builder1.AddFrom("users");
    query_builder1.AddWhere("status", "active");

    std::cout << query_builder1.BuildQuery() << "\n\n";

    //  AddWhere with map
    SqlSelectQueryBuilder query_builder2;
    query_builder2.AddColumn("product_id");
    query_builder2.AddColumn("product_name");
    query_builder2.AddFrom("products");

    std::map<std::string, std::string> conditions = {
        {"category", "electronics"},
        {"price", "100"},
        {"in_stock", "true"}
    };
    query_builder2.AddWhere(conditions);

    std::cout << query_builder2.BuildQuery() << "\n\n";

    return EXIT_SUCCESS;
}