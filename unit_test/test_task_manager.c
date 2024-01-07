#include <test_utils.h>
#include <extend/task_manager.h>

#define create_task() create_task((void*) 0x600000)

void test_task_manager_init() {
    init_task_manager();
    struct task_t* got_tasks = show_tasks();

    for (int index = 0; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_task_creation1() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We check if it was created
    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(0, got_tasks[0].task_id, "Task id should be 0 since this is the first id...");
    assert_equals_uint32(0x400000, got_tasks[0].first_page_address, "Should be first user address (0x400000)...");
    assert_equals_uint32(0, got_tasks[0].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[0].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(1, got_tasks[0].is_alive, "Task should be alive...");

    for (int index = 1; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_task_creation2() {
    // We init task manager
    init_task_manager();

    // We create 10 tasks
    for (int i = 0; i < 10; i++) {
        int create_res = create_task();
        assert_equals_int(0, create_res, "Task should be created...");
    }

    // We check if it was created
    struct task_t* got_tasks = show_tasks();
    for (int index = 0; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(index, task.task_id, "Task id should be the task number since they are created one after the other...");
        assert_equals_uint32(0x400000 + 0x1000 * index, task.first_page_address, "Should be 0x400000 + (0x1000 * task number)...");
        assert_equals_uint32(0, task.second_page_address, "Null since no second page...");
        assert_equals_uint8(0, task.has_second_page, "No second page by default...");
        assert_equals_uint8(5, task.quantum, "Should be default quantum number (5)...");
        assert_equals_uint8(1, task.is_alive, "Task should be alive...");
    }
}

void test_more_10_tasks() {
    // We init task manager
    init_task_manager();

    // We create 10 tasks
    for (int i = 0; i < 10; i++) {
        int create_res = create_task();
        assert_equals_int(0, create_res, "Task should be created...");
    }

    // We try to create an 11th task
    int create_res = create_task();
    assert_equals_int(1, create_res, "Task should not be created since it's the 11th...");
}

void test_ask_new_page() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We ask a second page
    int ask_res = ask_second_user_page(0);
    assert_equals_int(0, ask_res, "Page should have been given...");

    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(0, got_tasks[0].task_id, "Task id should be 0 since this is the first id...");
    assert_equals_uint32(0x400000, got_tasks[0].first_page_address, "Should be first user address (0x400000)...");
    assert_equals_uint32(0x401000, got_tasks[0].second_page_address, "Should be second user address (0x401000)...");
    assert_equals_uint8(1, got_tasks[0].has_second_page, "Second page asked...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(1, got_tasks[0].is_alive, "Task should be alive...");

    for (int index = 1; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_ask_new_page2() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We create a second task that will ask for a second page
    create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    int ask_res = ask_second_user_page(1);
    assert_equals_int(0, ask_res, "Page should have been given...");

    // We ask a second page
    ask_res = ask_second_user_page(0);
    assert_equals_int(0, ask_res, "Page should have been given...");

    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(0, got_tasks[0].task_id, "Task id should be 0 since this is the first id...");
    assert_equals_uint32(0x400000, got_tasks[0].first_page_address, "Should be first user address (0x400000)...");
    assert_equals_uint32(0x403000, got_tasks[0].second_page_address, "Should be 4th address given (0x403000)...");
    assert_equals_uint8(1, got_tasks[0].has_second_page, "Second page asked...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(1, got_tasks[0].is_alive, "Task should be alive...");

    for (int index = 2; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_ask_new_page_yet_again() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We ask a second page
    int ask_res = ask_second_user_page(0);
    assert_equals_int(0, ask_res, "Page should have been given...");

    // We ask a second page again
    ask_res = ask_second_user_page(0);
    assert_equals_int(1, ask_res, "Page already given...");
}

void test_ask_new_page_unknown_task() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We ask a second page
    int ask_res = ask_second_user_page(10);
    assert_equals_int(1, ask_res, "Task do not exist...");
}

void test_ask_new_page_dead_task() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We end it
    int end_res = end_task(0);
    assert_equals_int(0, end_res, "Task should be ended...");

    // We ask a second page
    int ask_res = ask_second_user_page(0);
    assert_equals_int(1, ask_res, "Task is already dead...");
}

void test_ask_new_page_no_task() {
    // We init task manager
    init_task_manager();

    // We ask a second page when no task created
    int ask_res = ask_second_user_page(0);
    assert_equals_int(1, ask_res, "No task created...");
}

void test_ask_new_page_new_task() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");
    
    // We ask a second page
    int ask_res = ask_second_user_page(0);
    assert_equals_int(0, ask_res, "Page should be given...");

    // We end it
    int end_res = end_task(0);
    assert_equals_int(0, end_res, "Task should be ended...");

    // We create a new task
    create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(1, got_tasks[0].task_id, "Second task created...");
    assert_equals_uint32(0x401000, got_tasks[0].first_page_address, "Second page pushed first (0x401000)...");
    assert_equals_uint32(0, got_tasks[0].second_page_address, "New task so no second page...");
    assert_equals_uint8(0, got_tasks[0].has_second_page, "No second page asked yet...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(1, got_tasks[0].is_alive, "Task should be alive...");

    for (int index = 1; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_ask_shared_page() {
    // TODO
}

void test_ask_22_shared_pages() {
    // TODO
}

void test_task_end1() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We end it
    int end_res = end_task(0);
    assert_equals_int(0, end_res, "Task should be ended...");

    // We check if it was ended
    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(0, got_tasks[0].task_id, "Task id should be 0 since this is the first id...");
    assert_equals_uint32(0x400000, got_tasks[0].first_page_address, "Should be first user address (0x400000)...");
    assert_equals_uint32(0, got_tasks[0].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[0].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(0, got_tasks[0].is_alive, "Task should be dead...");

    for (int index = 1; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_task_end2() {
    // We init task manager
    init_task_manager();

    // We create 2 tasks
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We end the second
    int end_res = end_task(1);
    assert_equals_int(0, end_res, "Task should be ended...");

    // We check if it was ended
    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(0, got_tasks[0].task_id, "Task id should be 0 since this is the first id...");
    assert_equals_uint32(0x400000, got_tasks[0].first_page_address, "Should be first user address (0x400000)...");
    assert_equals_uint32(0, got_tasks[0].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[0].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(1, got_tasks[0].is_alive, "Task should be alive...");

    assert_equals_uint64(1, got_tasks[1].task_id, "Task id should be 1 since this is the second task...");
    assert_equals_uint32(0x401000, got_tasks[1].first_page_address, "Should be second user address (0x401000)...");
    assert_equals_uint32(0, got_tasks[1].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[1].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[1].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(0, got_tasks[1].is_alive, "Task should be dead...");

    for (int index = 2; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_task_end3() {
    // We init task manager
    init_task_manager();

    // We create 2 task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We end them
    int end_res = end_task(0);
    assert_equals_int(0, end_res, "Task should be ended...");

    end_res = end_task(1);
    assert_equals_int(0, end_res, "Task should be ended...");

    // We check if it was ended
    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(0, got_tasks[0].task_id, "Task id should be 0 since this is the first id...");
    assert_equals_uint32(0x400000, got_tasks[0].first_page_address, "Should be first user address (0x400000)...");
    assert_equals_uint32(0, got_tasks[0].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[0].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(0, got_tasks[0].is_alive, "Task should be dead...");

    assert_equals_uint64(1, got_tasks[1].task_id, "Task id should be 1 since this is the second task...");
    assert_equals_uint32(0x401000, got_tasks[1].first_page_address, "Should be second user address (0x401000)...");
    assert_equals_uint32(0, got_tasks[1].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[1].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[1].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(0, got_tasks[1].is_alive, "Task should be dead...");

    for (int index = 2; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_task_end_recreate() {
    // We init task manager
    init_task_manager();

    // We create 2 tasks
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We end the first
    int end_res = end_task(0);
    assert_equals_int(0, end_res, "Task should be ended...");

    // We create a new one
    create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We check if it was ended
    struct task_t* got_tasks = show_tasks();
    assert_equals_uint64(2, got_tasks[0].task_id, "Task id should be 2 since this is the third task...");
    assert_equals_uint32(0x400000, got_tasks[0].first_page_address, "Should be first user address (0x400000)...");
    assert_equals_uint32(0, got_tasks[0].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[0].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[0].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(1, got_tasks[0].is_alive, "Task should be alive...");

    assert_equals_uint64(1, got_tasks[1].task_id, "Task id should be 1 since this is the second task...");
    assert_equals_uint32(0x401000, got_tasks[1].first_page_address, "Should be second user address (0x401000)...");
    assert_equals_uint32(0, got_tasks[1].second_page_address, "Null since no second page...");
    assert_equals_uint8(0, got_tasks[1].has_second_page, "No second page by default...");
    assert_equals_uint8(5, got_tasks[1].quantum, "Should be default quantum number (5)...");
    assert_equals_uint8(1, got_tasks[1].is_alive, "Task should be alive...");

    for (int index = 2; index < 10; index++) {
        struct task_t task = got_tasks[index];
        assert_equals_uint64(0, task.task_id, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.first_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint32(0, task.second_page_address, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.has_second_page, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.quantum, "Field should be 0 since there is no task...");
        assert_equals_uint8(0, task.is_alive, "Field should be 0 since there is no task...");
    }
}

void test_remove_non_existant() {
    // We init task manager
    init_task_manager();

    // We end it
    int end_res = end_task(10);
    assert_equals_int(1, end_res, "Task do not exist...");
}

void test_remove_dead_task() {
    // We init task manager
    init_task_manager();

    // We create a task
    int create_res = create_task();
    assert_equals_int(0, create_res, "Task should be created...");

    // We end it
    int end_res = end_task(0);
    assert_equals_int(0, end_res, "Task should be ended...");

    // We end it again
    end_res = end_task(0);
    assert_equals_int(1, end_res, "Task should be already dead...");
}

void test_remove_0_task() {
    // We init task manager
    init_task_manager();

    // We end task 0 when no task created
    int end_res = end_task(0);
    assert_equals_int(1, end_res, "Task do not exist yet...");
}

int main() {
    test_task_manager_init();
    test_task_creation1();
    test_task_creation2();
    test_more_10_tasks();
    test_ask_new_page();
    test_ask_new_page2();
    test_ask_new_page_yet_again();
    test_ask_new_page_unknown_task();
    test_ask_new_page_dead_task();
    test_ask_new_page_no_task();
    test_ask_new_page_new_task();
    test_ask_shared_page();
    test_ask_22_shared_pages();
    test_task_end1();    
    test_task_end2();    
    test_task_end3();
    test_task_end_recreate();
    test_remove_non_existant();
    test_remove_dead_task();
    test_remove_0_task();
    
    return 0;
}
