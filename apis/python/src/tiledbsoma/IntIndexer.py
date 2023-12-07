from typing import Union

import numpy as np

from . import pytiledbsoma as clib
from ._experiment import _indexer_map_locations
from .options import SOMATileDBContext

"""External API"""


class IntIndexer:
    @staticmethod
    def map_locations(
        keys: np.typing.NDArray[np.int64],
        context: Union[SOMATileDBContext, None] = None,
    ) -> clib.IntIndexer:  # type ignore
        return _indexer_map_locations(keys, context)


"""For internal use"""
