from LambdaData import *


def get_help(group_title, functions):
    print(f"\n## {group_title}\n")
    for func in functions:
        print(f"### `LambdaData.{func.__name__}`")
        print("```")
        help(func)
        print("```")


if __name__ == '__main__':
    get_help("Generators", (iota, generate, generate_n))
    get_help("Expansions", (fork, inclusive_scan, exclusive_scan))
    get_help("Transforms", (transform, adjacent_difference, partial_sum))
    get_help("Permutations", (partition, ))
    get_help("Reductions", (
        reduce, accumulate, product, min_max, star_sum, star_product))
    get_help("Queries", (all_of, any_of, none_of))
    get_help("Transform & Reduce", (transform_reduce, inner_product, matrix_multiply))
    get_help("Multidimensional Reductions", (zip_transform, transposed_sums))
    get_help("Multi-Set Operations", (union, intersection, difference, symmetric_difference))
    get_help("Pandas Helpers", (value_span, star_cat_row, star_cat_col))
    get_help("Randomizers", (random_range, shuffle, TruffleShuffle))
