let design = {
    "$schema": "https://vega.github.io/schema/vega/v5.json",
    "width": 400,
    "height": 200,
    "padding": 5,
    "autosize": { "type": "fit", "resize": true },

    "data": [
      {
          "name": "dimensions"
      }
    ],

    "scales": [
        {
            "name": "xscale",
            "type": "band",
            "domain": { "data": "dimensions", "field": "name" },
            "range": "width",
            "padding": 0.05,
            "round": true
        },
        {
            "name": "yscale",
            "domain": { "data": "dimensions", "field": "avg" },
            "nice": true,
            "range": "height"
        }
    ],

    "axes": [
        { "orient": "bottom", "scale": "xscale" },
        { "orient": "left", "scale": "yscale" }
    ],


    "marks": [
        {
            "type": "rect",
            "from": { "data": "dimensions" },
            "encode": {
                "enter": {
                    "x": { "scale": "xscale", "field": "name" },
                    "width": { "scale": "xscale", "band": 1 },
                    "y": { "scale": "yscale", "field": "avg" },
                    "y2": { "scale": "yscale", "value": 0 }
                },
                "update": {
                    "fill": { "value": "steelblue" }
                },
                "hover": {
                    "fill": { "value": "red" }
                }
            }
        }
    ]
}