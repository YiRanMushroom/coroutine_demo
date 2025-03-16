import ywl.prelude;

void delete_int(const int *p) {
    ywl::print_ln("delete_int: ", *p).flush();
    delete p;
}

constexpr int func1() {
    ywl::print_ln("func1").flush();
    return 42;
}

ywl::miscellaneous::coroutine::generator_task<int()>
int_generator() {
    int i = 0;
    while (true) {
        co_yield i++;
    }
}

ywl::miscellaneous::coroutine::generator_task<int(int)>
identical_generator() {
    while (true) {
        co_yield co_await nullptr; // NOLINT
    }
}

ywl::miscellaneous::coroutine::generator_task<uint32_t()>
fibonacci_generator() {
    uint64_t a = 0, b = 1;
    while (true) {
        if (a > std::numeric_limits<uint32_t>::max()) {
            co_return;
        }
        co_yield static_cast<uint32_t>(a);
        auto c = a + b;
        a = b;
        b = c;
    }
}

ywl::miscellaneous::coroutine::generator_task<uint32_t(uint32_t), uint32_t>
ident_feedback_gen() {
    static auto random_gen = ywl::miscellaneous::random_generator_int(0, 100);
    while (true) {
        auto i = co_await random_gen();
        co_yield i;
    }
}

int main() {
    std::unique_ptr<int> uniptr = std::make_unique<int>(42);

    std::shared_ptr<int> shptr = std::make_shared<int>(42);
    auto cp{shptr};

    constexpr decltype(&func1) fn1_ptr = ywl::basic::function_t<func1>{};

    int x = fn1_ptr();

    auto generator = int_generator();
    for (int i = 0; i < 10; ++i) {
        ywl::print_ln(generator.yield_value()).flush();
    }

    auto identical = identical_generator();
    for (int i = 0; i < 10; ++i) {
        ywl::print_ln(identical.yield_value(i)).flush();
    }

    auto fibonacci = fibonacci_generator();
    while (fibonacci) {
        if (auto optional = fibonacci.yield_optional()) {
            ywl::util::print(*optional, "");
        } else {
            break;
        }
    }

    std::vector<int> vec{1, 2, 3, 4, 5};
    auto res = vec | std::views::transform([](int i) { return i * i; })
               | std::views::filter([](int i) { return i % 2 == 0; })
               | std::views::take(2);

    auto list = res | std::ranges::to<std::list<int>>();

    ywl::print_ln(list).flush();

    auto feedback_gen = ident_feedback_gen();
    feedback_gen.set_feedback_callback([](uint32_t i) {
        ywl::print_ln("Feedback: ", i).flush();
    });

    for (int i = 0; i < 10; ++i) {
        ywl::print_ln(feedback_gen.yield_value(i)).flush();
    }
}
