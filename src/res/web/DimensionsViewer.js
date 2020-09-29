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

        // Point value range
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
                    "field": "name",
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

        // Average point value
        {
            "mark": {
                "type": "line",
            },
            "encoding": {
                "x": {
                    "field": "name",
                    "type": "ordinal",
                },
                "y": {
                    "field": "avg",
                    "type": "quantitative",
                },
                "tooltip": [
                    { "field": "avg", "type": "quantitative", "title": "Average point value" },
                ]
            }
        },

        /*
        {
            "mark": {
                "type": "bar",
                //"point": true
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
                    { "field": "min", "type": "quantitative", "title": "Minimum" },
                    { "field": "max", "type": "quantitative", "title": "Maximum" },
                    { "field": "avg", "type": "quantitative", "title": "Average" },
                    { "field": "mean", "type": "quantitative", "title": "Mean" },
                    { "field": "name", "type": "nominal", "title": "Dimension" }
                ]
            }
        },

        {
            "mark": {
                "type": "point",
                //"shape": "stroke",
                //"filled": true,
                //"color": "black",

                "size": 50
            },
            "encoding": {
                "x": {
                    "field": "id",
                    "type": "ordinal",
                },
                "y": {
                    "field": "min",
                    "type": "quantitative"
                },
                "tooltip": [
                    { "field": "min", "type": "quantitative", "title": "Minimum point value" },
                ]
            }
        }


        /*,
        {
            "mark": {
                "type": "errorbar",
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
                "type": "point",
                "filled": true,
            },
            "encoding": {
                "x": {
                    "field": "id",
                    "type": "ordinal",
                },
                "y": {
                    "field": "max",
                    "type": "quantitative"
                },
                "tooltip": [
                    { "field": "max", "type": "quantitative", "title": "Maximum point value" },
                ]
            }
        }
        */
        
    ]
}