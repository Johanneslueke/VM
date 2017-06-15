#pragma once

/*

"Traditional" visitation
============================

	Visiting a variant is usually done by writing a visitor struct / class outside of the scope where the variant 
	is actually being visited.The visitor contains a set of overloads that will match the types a variant can hold.
	As an example, let's say that we have a variant type alias --> vnum <-- that can hold one of several numerical 
	types:

		// Can hold either an `int`, a `float` or a `double`.
		using vnum = vr::variant<int, float, double>;
		We want to to visit this variant and print...

	"$i" for integers.

	"$f" for floats.

	"$d" for doubles.

	...where $ is the currently stored value.

	To achieve that, it's sufficient to write a vnum_printer struct with an operator() overload for every type 
	supported by our variant:

		struct vnum_printer
		{
			void operator()(int x) { cout << x << "i\n"; }
			void operator()(float x) { cout << x << "f\n"; }
			void operator()(double x) { cout << x << "d\n"; }
		};
	Afterwards, we can simply invoke vr::visit using an instance of vnum_printer as the first argument, and an 
	instance of the vnum variant as the second one :

		// Prints "0i".
		vnum v0{ 0 };
		vr::visit(vnum_printer{}, v0);

		// Prints "5f".
		v0 = 5.f;
		vr::visit(vnum_printer{}, v0);

		// Prints "33.51d".
		v0 = 33.51;
		vr::visit(vnum_printer{}, v0);
	
	This works, but requires us to define a new vnum_printer visitor type - this boilerplate can be avoided.

*/

namespace vm
{namespace helper {
		namespace impl {
			template <typename...>
			struct overload_set;
			/*
				Base Case
			*/
			template <typename TF>
			struct overload_set<TF> : TF
			{
				using call_type = TF;
				using call_type::operator();

				template<typename TFFwd>
				overload_set(TFFwd&& f) noexcept : TF{ std::forward<TFFwd>(f) }
				{
				}
			};

			/*
				Recursive Case
			*/
			template <typename TF, typename... TFs>
			struct overload_set<TF, TFs...> : TF, overload_set<TFs...>
			{

				using f_type = TF;
				using call_type = overload_set;


				template<typename TFFwd, typename ... TRest>
				overload_set(TFFwd &&f, TRest &&... fs) noexcept
					: f_type(std::forward<TFFwd>(f)),
					overload_set<TFs ...>{ std::forward<TRest>(fs) ...}
				{
				}

				using f_type::operator();
				using overload_set<TFs ...>::operator();
			};

			template<typename TF>
			struct y_combinator_wrapper {
				TF _f;

				template<typename TFFwd>
				y_combinator_wrapper(TFFwd&&  xs) : _f{ std::forward<TFFwd>(xs) }
				{

				}

				template<typename ... Ts>
				decltype(auto) operator()(Ts&& ... xs) {
					return _f(std::ref(*this),
						std::forward<Ts>(xs) ...);
				}
			};

			template<typename TF>
			auto y_combinator(TF&& f) {
				return y_combinator_wrapper<std::decay_t<TF>>(std::forward<TF>(f));
			}

			template<typename TVisitor, typename... TVariants>
			decltype(auto) visit_recursively(TVisitor&& visitor, TVariants&& ... variants)
			{
				return std::visit(
					std::forward<TVisitor>(visitor),
					std::forward<decltype(variants)>(variants)._data...);
			}
		}

		
		/**
		* Designed after Propsal P0051R2
		* https://github.com/viboes/tags/blob/master/doc/proposals/overload/P0051R2.md
		*
		* Allow to overload lambdas or function objects!
		*
		* Pros: 
		*	-> With overload the user can use existing functions that it can combine, using the function object
		*	   would need to define an overload and forward to the existing function.
		*
		*	-> The user can group the overloaded functions as close as possible where they are used and don't 
		*	   need to define a class elsewhere. This is in line with the philosophy of lambda functions.
		*
		*	-> Each overload can have its own captured data, either using lambdas or other existing function 
		*	   objects.
		*
		*	-> Any additional feature of lambda functions, automatic friendship, access to this, and so forth.
		*
		* Cons:
		*	-> The overload function generates a function object that is a little bit more complex and so would 
		*	   take more time to compile.
		*
		*   -> The the result type of overload function is unspecified and so storing it in an structure is more 
		*      difficult (as it is the case for std::bind).
		*
		*	-> With the function object the user is able to share the same data for all the overloads. Note that
		*      that the last point could be seen as an advantage and a liability depending on the user needs.
		*
		*/
		template <typename... TFs>
		constexpr auto overload(TFs && ... fs) noexcept
		{
			return impl::overload_set<
				std::remove_reference_t<TFs>
				...>{std::forward<TFs>(fs)...};
		}

		template<typename TReturn, typename... TFs>
		auto recursive_visitor(TFs&&...fs)
		{
			return impl::y_combinator(
			[o = overload(std::forward<decltype(fs)>(fs)...)]
			
			(auto self, auto&&... xs) -> TReturn 
			{
				return o([&self](auto&& ... vs) {
					return impl::visit_recursively(self, std::forward<decltype(vs)>(vs)...);
				
				}
				, std::forward<decltype(xs)>(xs) ...);
			});
		}



		/**
		*
		*/
		template<typename ... TVariants>
		constexpr auto match(TVariants&& ... vs) 
		{
			return [&vs ...](auto&& ... fs) -> decltype(auto) {

				auto visitor = overload(std::forward<decltype(fs)>(fs)...);

				return std::visit(visitor,
					std::forward< decltype(vs)>(vs)...);

			};
		}

		template<typename ... TVariants>
		constexpr auto match_recursively(TVariants&& ... vs)
		{
			return[&vs ...](auto&& ... fs) -> decltype(auto) {

				auto visitor = recursive_visitor(std::forward<decltype(fs)>(fs)...);

				return impl::visit_recursively(visitor,
					std::forward< decltype(vs)>(vs)...);

			};
		}
	}
}