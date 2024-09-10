#include "entity.h"

static array_list_t *entity_list;
static array_list_t *active_entity_id_list;

void entity_init(void) {

	entity_list = array_list_new(sizeof(entity_t), 0);
	active_entity_id_list = array_list_new(sizeof(size_t), 0);
	
}

size_t entity_create(vec2 position, vec2 size, vec2 velocity, u8 collision_layer, u8 collision_mask, on_hit_f on_hit, on_hit_static_f on_hit_static, u8 is_active) {
	
	entity_t new_entity = {
		.body_id = physics_body_create(position, size, velocity, collision_layer, collision_mask, on_hit, on_hit_static)
	};
	
	size_t entity_id = array_list_push(entity_list, &new_entity);
	
	if(is_active)
		array_list_push(active_entity_id_list, &entity_id);
	
	return entity_id;
	
}

entity_t *entity_get(size_t id) {
	return (entity_t *)array_list_get(entity_list, id);
}

size_t entity_count(void) {
	return entity_list->length;
}