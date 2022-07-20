export module flow.core.yuma.by_ptr;

import flow.core.yuma.concepts;

export namespace flow::core::yuma
{
	template <uintptr_t Addr, class TRet = void>
	struct by_ptr final
	{
		template<class... TArgs>
		[[nodiscard]] constexpr static auto invoke(TArgs&&... _args) // -> TRet
		{
			constexpr auto size = sizeof...(TArgs);

			if constexpr (size > 0) {
				constexpr auto tuple = std::make_tuple(transform(_args)...);
				return _invoke(std::forward<decltype(tuple)>(tuple));
			}

			return _invoke();
		}

		template <class T>
		[[nodiscard]] constexpr static auto transform(T _value)
		{
			if constexpr (concepts::have_data<T>) {
				return reinterpret_cast<const char*>(std::data(_value));
			} else if constexpr (std::is_arithmetic_v<T>) {
				return _value;
			} else {
				static_assert(true);
			}
		}

	private:
		[[nodiscard]] constexpr static auto _invoke(void) -> TRet
		{
			using type = auto(void)->TRet;
			return std::invoke((type*)Addr);
		}

		template<class... T>
		[[nodiscard]] constexpr static auto _invoke(std::tuple<T...>&& _value) -> TRet
		{
			using type = auto(T...)->TRet;
			return std::apply((type*)Addr, _value);
		}
	};
}
