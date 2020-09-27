let design = {
    "$schema": "https://vega.github.io/schema/vega-lite/v4.json",
    "width": "container",
    "height": "container",
    "autosize":
    {
        "type": "fit",
        "resize": true,
        //"contains": "padding"
    },
    /*
    "autosize": {
        "type": "fit",
        "resize": true,
        "contains": "padding"
    },
    */
    "padding": 20,
    "data": {
        "name": "dimensions"
    },
    "layer": [
        {
            "mark": {
                "type": "errorband",
                "borders": {
                    "opacity": 0.3,
                    "strokeDash": [2, 2]
                }
            },
            "encoding": {
                "x": {
                    "field": "id",
                    "type": "ordinal"
                },
                "y": {
                    "field": "max",
                    "type": "quantitative",
                    //"scale": { "zero": false },
                    "title": "Point value"
                },
                "y2": { "field": "min" },
                "tooltip": [
                    { "field": "min", "type": "quantitative", "title": "Minimum" },
                    { "field": "max", "type": "quantitative", "title": "Maximum" },
                    { "field": "avg", "type": "quantitative", "title": "Average" },
                    { "field": "mean", "type": "quantitative", "title": "Mean" },
                    { "field": "name", "type": "nominal", "title": "Dimension" },
                ]
            }
        },
        {
            "mark": {
                "type": "line",
                "point": true
            },
            "encoding": {
                "x": {
                    "field": "id",
                    "type": "ordinal"
                },
                "y": {
                    "field": "avg",
                    "type": "quantitative"
                },
                "tooltip": [
                    { "field": "avg", "type": "quantitative", "title": "Average" },
                ]
            }
        }/*,
        {
            "mark": {
                "type": "line",
                "strokeWidth": 0.5,
                "strokeDash": [6, 4]
            },
            "encoding": {
                "x": {
                    "field": "id",
                    "type": "quantitative",
                    "title": "Dimension"
                },
                "y": {
                    "field": "mean",
                },
                "tooltip": [
                    { "field": "mean", "type": "quantitative", "title": "Mean" },
                ]
            }
        }*/
    ]
}