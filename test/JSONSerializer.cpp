#include "catch.hpp"

#include <chig/Context.hpp>
#include <chig/GraphFunction.hpp>
#include <chig/LangModule.hpp>

using namespace chig;
using namespace nlohmann;

TEST_CASE("JsonSerializer", "[json]")
{
	GIVEN("A default constructed Context with a LangModule and GraphFunction named hello") {
		
		Context c;
		c.addModule(std::make_unique<LangModule>(c));
		
		GraphFunction func(c, "hello");

		THEN("The JSON should be correct") {
			auto correctJSON = R"ENDJSON(
				{
					"type": "function",
					"name": "hello",
					"nodes": [],
					"connections": []
				}
			)ENDJSON"_json;
			
			REQUIRE(correctJSON == func.toJSON());
		}
		
		WHEN("We create some nodes and try to dump json") {
			
			
			std::vector<std::pair<llvm::Type*, std::string>> inputs = {
				{llvm::Type::getInt32Ty(c.context), "in1"}};
			
			auto entry = func.insertNode(std::make_unique<EntryNodeType>(c, inputs), 32, 32);
			
			THEN("The JSON should be correct") {
				auto correctJSON = R"ENDJSON(
					{
						"type": "function",
						"name": "hello",
						"nodes": [
							{
							"type": "lang:entry",
							"location": [32.0,32.0],
							"data": {
								"in1": "lang:i32"
							}
							}
						],
						"connections": []
					}
					)ENDJSON"_json;
				
				REQUIRE(correctJSON == func.toJSON());
			}
			
			WHEN("A lang:if is added") {
				auto ifNode = func.insertNode(c.getNodeType("lang", "if", {}), 44.f, 23.f);
				
				THEN("The JSON should be correct") {
					auto correctJSON = R"ENDJSON(
						{
							"type": "function",
							"name": "hello",
							"nodes": [
								{
									"type": "lang:entry",
									"location": [32.0,32.0],
									"data": {
										"in1": "lang:i32"
									}
								},
								{
									"type": "lang:if",
									"location": [44.0, 23.0],
									"data": null
								}
							],
							"connections": []
						}
						)ENDJSON"_json;
					
					REQUIRE(correctJSON == func.toJSON());
				}
				
			}
			
		
			
		/*	
			connectExec(*entry, 0, *ifNode, 0);
			connectData(*entry, 0, *ifNode, 0);
			
			
			auto dumpedJSON = func.toJSON();
			
			THEN("It should be the same as the sample!") {
				

				auto correctJSON = R"ENDJSON(
{
  "type": "function",
  "name": "hello",
  "nodes": [
    {
      "type": "lang:entry",
      "location": [32.0,32.0],
      "data": {
        "in1": "lang:i32"
      }
    },
    {
      "type": "lang:if",
      "location": [44.0,23.0],
      "data": null
    }
  ],
  "connections": [
    {
      "type": "exec",
      "input": [0,0],
      "output": [1,0]
    },
    {
      "type": "data",
      "input": [0,0],
      "output": [1,0]
    }
  ]
}
)ENDJSON"_json;
		
				REQUIRE(dumpedJSON == correctJSON);
			}
*/
		}
		
	} 

}