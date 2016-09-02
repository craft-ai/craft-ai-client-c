/** Copyright (c) 2016, craft ai All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of craft ai nor the names of
 *    its contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "craftai.h"

craft_status_t craft_init() {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_teardown() {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_set_config(craft_settings_t *config) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_create_agent(craft_model_t *model, char *agent_id) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}
craft_status_t craft_get_agent(char *agent_id) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}
craft_status_t craft_delete_agent(char *agent_id) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_add_operations(char *agent_id, craft_context_t *operations_list, int nb_operations) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}
craft_status_t craft_list_operations(char *agent_id, craft_context_t *operations_list, int nb_operations) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}
craft_status_t craft_retrieve_state(char *agent_id, time_t timestamp, craft_context_t *state) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_compute_decision_tree(char *agent_id, time_t timestamp, craft_tree_t *tree) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}
craft_status_t craft_decide(craft_tree_t *tree, craft_context_t *context, craft_decision_t *decision) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}