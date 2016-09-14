#include "unit_tests.h"

#include "craftai.h"

#include <stdio.h>

CRAFT_TEST(create_destroy_agent) {
    CRAFT_TEST_ASSERT_CALL_OK(craft_init());

    craft_model_t model;
    model.properties[0].key = "externalTemp";
    model.properties[0].type = CONTINUOUS;
    model.properties[1].key = "time";
    model.properties[1].type = TIME_OF_DAY;
    model.properties[2].key = "internalTemp";
    model.properties[2].type = ENUM;
    model.properties_count = 3;

    model.outputs[0] = "internalTemp";
    model.outputs_count = 1;

    model.time_quantum = 60;

    char* agent_id = "client_c_test_agent";

    CRAFT_TEST_ASSERT_CALL_OK(craft_delete_agent(agent_id));

    CRAFT_TEST_ASSERT_CALL_OK(craft_create_agent(&model, agent_id));
    CRAFT_TEST_ASSERT_STR_EQ(agent_id, "client_c_test_agent");

    CRAFT_TEST_ASSERT_CALL_OK(craft_teardown());
    return 0;
}

CRAFT_TEST_MAIN() {
    CRAFT_TEST_RUN(create_destroy_agent);
    return 0;
}
