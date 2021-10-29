message("-- Linking get_best_assignments")
add_library(task_assignment get_best_assignments.dll)
set_target_properties(task_assignment PROPERTIES LINKER_LANGUAGE C)