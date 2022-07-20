export module flow.core.yuma.concepts;

export namespace flow::core::yuma::concepts
{
	template <class T>
	concept have_data = requires (T t) { t.data(); };
}
