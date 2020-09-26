let design = {
    "$schema": "https://vega.github.io/schema/vega/v5.json",
    "width": 400,
    "height": 200,
    "padding": 5,
    "autosize": { "type": "fit", "resize": true },

    "data": [
      {
          "name": "counts"
      }
    ],

    "scales": [
      {
          "name": "yscale",
          "type": "linear",
          "range": "height",
          "domain": {
              "data": "counts",
              "field": "count"
          }
      },
      {
          "name": "xscale",
          "type": "band",
          "domain": {
              "data": "counts",
              "field": "bin0"
          },
          "range": "width",
          "round": true,
          "nice": true
      }
    ],

    "axes": [
        {
            "orient": "left",
            "scale": "yscale",
            "grid": true,
            "tickCount": 5,
            "format": "s",
            "title": "Count"
        },
        {
            "orient": "bottom",
            "scale": "xscale",
            "tickCount": 1,
            "labelOverlap": "parity",
            "round": true
        }
    ],


    "marks": [
      {
          "type": "rect",
          "from": { "data": "counts" },
          "encode": {
              "update": {
                  "x": { "scale": "xscale", "field": "bin0", "offset": 1 },
                  "x2": { "scale": "xscale", "field": "bin1" },
                  "y": { "scale": "yscale", "field": "count" },
                  "y2": { "scale": "yscale", "value": 0 },
                  "fill": { "value": "steelblue" }
              },
              "hover": {
                  "fill": { "value": "firebrick" }
              }
          }
      }
    ]
}