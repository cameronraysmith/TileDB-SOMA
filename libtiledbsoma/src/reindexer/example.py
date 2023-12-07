import os
from time import perf_counter

import cellxgene_census

import tiledbsoma as soma

# soma.pytiledbsoma.config_logging("debug")
print(os.getpid())


def main():
    census_s3 = dict(census_version="latest")
    t1 = perf_counter()
    with cellxgene_census.open_soma(**census_s3) as census:
        with census["census_data"]["homo_sapiens"].axis_query(
            measurement_name="RNA",
            obs_query=soma.AxisQuery(value_filter="""tissue_general == 'eye'"""),
        ) as query:
            query.to_anndata(X_name="raw")
    t2 = perf_counter()
    print(f"End to end time {t2 - t1}")


main()
