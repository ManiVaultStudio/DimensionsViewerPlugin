let design = {
    "$schema": "https://vega.github.io/schema/vega-lite/v4.json",
    "width": "container",
    "height": "container",
    "title": {
        "text": []
    },
    "config": {
        "axis": {
            "grid": true,
            "gridColor": "#dedede"
        }
    },
    "padding": 10,
    "autosize":
    {
        "type": "fit",
        "resize": true,
    },
    "data": {
        "name": "dimensions",
        "values": []
    },
    "layer": []
}

function getRangeMark(channel, strokeWidth) {
    return {
        "mark": {
            "type": "errorband",
            //"borders": {
            //    "opacity": 0.5,
            //    "strokeWidth": strokeWidth,
            //},
            "opacity": channel.opacity,
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": "min",
                "type": "quantitative",
                "title": "Point value"
            },
            "y2": {
                "field": "max",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getAggregateLineMark(channel, strokeWidth, strokeDash) {
    return {
        "mark": {
            "type": "line",
            "strokeWidth": strokeWidth,
            "strokeDash": strokeDash,
            "strokeJoin": "round",
            "opacity": 1
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": "agg",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getAggregatePointsMark(channel) {
    return {
        "mark": {
            "type": "point",
            "fill": channel.color,
            "opacity": 1,
            "size": 8
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": "agg",
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function getStdDevLineMark(channel, field, strokeWidth, strokeDash) {
    return {
        "mark": {
            "type": "line",
            "strokeWidth": strokeWidth,
            "strokeDash": strokeDash,
            "strokeJoin": "round",
            "opacity": 1
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": channel.encoding.x,
            "y": {
                "field": field,
                "type": "quantitative",
                "title": "Point value"
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

//function getHistogramMark(channel) {
//    return {
//        "mark": {
//            "type": "rect",
//            "cornerRadiusTopLeft": 3, "cornerRadiusTopRight": 3
//        },
//        "transform": [
//            {
//                "filter": {
//                    "field": "chn",
//                    "equal": channel.index
//                }
//            }
//        ],
//        "color": {
//            "x1": 1,
//            "y1": 1,
//            "x2": 1,
//            "y2": 0,
//            "gradient": "linear",
//            "stops": [
//                { "offset": 0.0, "color": "red" },
//                { "offset": 0.5, "color": "white" },
//                { "offset": 1.0, "color": "blue" }
//            ]
//        },
//        "encoding": {
//            "row": 1,
//            "x": channel.encoding.x,
//            "y": {
//                "value": 100
//            },
            
//        }
//    }
//}

function addChannel(design, channel) {

    design.data.values = design.data.values.concat(channel.dimensions);

    if (channel.profileType > 0) {
        design.layer.push(getAggregateLineMark(channel, 2, []));
        design.layer.push(getAggregatePointsMark(channel));
    }

    if (channel.bandType > 0) {
        design.layer.push(getStdDevLineMark(channel, "v1", 1, [2, 2]));
        design.layer.push(getStdDevLineMark(channel, "v2", 1, [2, 2]));
    }

    if (channel.showRange)
        design.layer.push(getRangeMark(channel, 1));

    //design.layer.push(getHistogramMark(channel));
}

function getDesign(spec) {

    design.data.values = [];
    design.layer = [];

    let titles = [];

    for (channelName in spec.channels) {
        let channel = spec.channels[channelName];

        titles.push(channel.datasetName);

        channel.encoding = {
            x: {
                field: spec.showDimensionNames ? 'dimName' : 'dimId',
                type: spec.showDimensionNames ? 'ordinal' : 'nominal',
                title: spec.showDimensionNames ? 'Dimension name' : 'Dimensions index',
                axis: {
                    labelAngle: spec.showDimensionNames ? -45 : 0
                }
            }
        }

        addChannel(design, channel);
    }

    design.title.text = `[${titles.join(', ')}]`;

    return design;
}